# 对角线约束支持的修改总结

## 主要新增/调整的逻辑（基于当前代码实现）

1. **在区间/区域等价与包含判断中加入“对角线约束蕴含”**
   - 位置：`include/tchecker/zg/details/zg.hh` 的 `equal(...)`。
   - 逻辑：先用原有 LU 语义（`_zone_semantics.equal`）判断非对角线部分；若 LU 认为“相等/被包含”，再对对角线约束做额外蕴含检查。
   - 等价（subsumption=false）：双向检查 `diagonal_implication(z1,z2)` 与 `diagonal_implication(z2,z1)`。
   - 包含（subsumption=true）：检查 `diagonal_implication(z2,z1)`（被包含端 → 包含端）。

2. **收集状态相关的对角线约束集合 G(q)**
   - 位置：`collect_diagonal_constraints(...)`。
   - 逻辑：
     - 对每个 location 的 invariant 用字节码解释器抽取约束；
     - 对每条 outgoing edge 的 guard 用字节码解释器抽取约束；
     - 只保留 `c.diagonal()` 的对角线约束。
   - 这相当于实现了一个“状态级 guard 集合” `G(q)`（由不变式与后继边 guard 组成）。

3. **对角线蕴含判定（DBM 对角项比较）**
   - 位置：`diagonal_implication(...)` + `zone_entails(...)`。
   - 逻辑：
     - 将对角线约束 `x_i - x_j <= c` 映射到 DBM 的 `(i,j)` 约束；
     - 用 `db_cmp(current, bound)` 判定 DBM 是否满足该对角线约束；
     - 对所有对角线约束做“如果 from 满足，则 to 也必须满足”的蕴含检查。

## 与 `todo.tex` 思路的一致性评估

整体 **部分一致**，核心思路已对上，但实现不完整/未显式化。

一致点：
- **“非对角线 LU + 对角线蕴含”** 的拆分与 `todo.tex` 第 4.1 节完全一致：
  - 代码中先用 LU 语义 `_zone_semantics.equal` 判断非对角线部分；
  - 再用 `diagonal_implication` 补充对角线部分。
- **G(q) 的来源**与 `todo.tex` 第 2/4 节一致：
  - 代码从 `invariant` 与 `outgoing guard` 抽取对角线约束，形成状态级 guard 集合。

未覆盖/未显式实现的点：
- `todo.tex` 强调的 **“有限布尔抽象/对角线类”** 并未在代码中显式构造或缓存；目前是逐次扫描并即时判断。
- `todo.tex` 提到的 **LU(G) 由 G(q) 提取** 未在此处显式体现；当前 LU 部分仍依赖原有 `_zone_semantics` 的实现与其外部传入的 LU bounds。
- 终止性与“强有限性”证明仅在文档层面存在，代码里没有对应的显式结构或注释。

## 结论

当前实现已经落地了 `todo.tex` 的核心结构性想法：
- “LU 处理非对角线 + 对角线蕴含补充”；
- “G(q) 来自 invariant/guard”。

但尚未完整体现 `todo.tex` 中关于 **G-simulation 的 finiteness/boolean abstraction/LU(G) 提取** 等细节，属于**概念一致、实现部分到位**的状态。

---

## 追加：后续问答记录

Q：相当于是静态分析收集所有对角线约束得到 G(q)，然后怎么利用 G(q) 来判断是否模拟？我关心的点在于不用 LU-extrapolation 的话，替代方案是如何且保证有效的。

A：
- **G(q) 的收集方式（静态分析）**：对当前控制点 q，把它所在 location 的 invariant 与所有 outgoing edge 的 guard 解析为 clock constraints，然后筛掉非对角线约束，仅保留对角线约束形成 `G(q)`（代码里是 `collect_diagonal_constraints`，把这些约束装进 `g1` / `g2`）。
- **如何用 G(q) 判定模拟（subsumption / equivalence）**：
  1) 先用原来的 **LU 语义** 判定非对角线部分（`_zone_semantics.equal`），若此部分已失败，直接不模拟；
  2) 再用 **对角线蕴含检查**：
     - subsumption：要求 `diagonal_implication(z2, z1, G)` 成立；
     - equivalence：要求 `diagonal_implication(z1, z2, G)` 与 `diagonal_implication(z2, z1, G)` 都成立。
  3) 其中 `diagonal_implication` 的语义是：对所有 `φ ∈ G(q)`，若 `from` 满足 φ，则 `to` 也必须满足 φ（用 DBM 对角项检查）。
- **不用 LU-extrapolation 的替代方案是什么？**
  - 不是完全“不用 LU”，而是**保留 LU 用于非对角线部分**，并**额外加入对角线蕴含检查**（即 G-simulation 的“二段式”）。
  - 这在逻辑上对应 `v ⊑^LU_G v'  ⇔  v ⊑_{LU(G)} v'  ∧  (对角线蕴含)`。
- **为什么是有效的（sound）？**
  - 非对角线部分仍由 LU-simulation 保证（经典 soundness）。
  - 对角线部分通过 G(q) 中的对角线 guard 蕴含，保证不会错误吞掉需要区分的对角线行为。
  - 组合后得到的关系是 G-simulation（理论上在 `todo.tex` 里引用 GAbs 论文证明 sound + finite）。


Q：G(q) 中的对角线 guard 蕴含，具体是怎么做的呢？其结果和 LU-extrapolation 并起来就是最后结果是吗？

A：
- **对角线蕴含的具体做法**：
  - 遍历 `G(q)` 中每个对角线约束 `φ`（形式是 `x_i - x_j # c`）。
  - 在 `zone_entails(zone, φ)` 中，用 DBM 的 `(i,j)` 项和 `c` 比较：
    - 取 `current = dbm[i,j]`，`bound = (#, c)`，如果 `current <= bound` 则 `zone` 满足 `φ`。
  - `diagonal_implication(from, to, G)` 的语义是：
    - 对所有 `φ ∈ G`，若 `from` 满足 `φ`，则 `to` 也必须满足 `φ`；
    - 只要存在某个 `φ` 违反该蕴含，就判定失败。
- **和 LU 部分如何合并**：
  - 是的，最终结果就是 **“LU 的非对角线判定” ∧ “对角线蕴含判定”**。
  - 在代码里是：
    - 先调用 `_zone_semantics.equal` 做 LU 判定（非对角线）；
    - 仅当 LU 判定通过时，才进一步做 `diagonal_implication`；
    - subsumption/ equivalence 的方向由是否需要双向蕴含决定。


Q：继续追加，详细说明对角线蕴含的做法中，zone、φ、from、to、G 分别是什么，这两个函数的返回值又是什么意思？

A：
- **术语/对象含义**：
  - `G`：在某个控制点 `q` 上静态收集到的对角线约束集合（来自该位置 invariant + outgoing guard），也可写作 `G(q)`。
  - `φ`：`G(q)` 中的单个对角线约束，形式为 `x_i - x_j # c`（`#` 是 `<,<=,>,>=` 中的比较符）。
  - `zone`：一个“区间/时钟区域”，在实现里由 DBM 表示（`zone.dbm()`）。
  - `from` / `to`：两个 zone（`zone_t`），分别表示“蕴含关系的前件/后件”。
    - `from` 是我们要求“保持”的对角线约束来源；
    - `to` 是被比较的目标 zone。

- **`zone_entails(zone, φ)` 的返回值**：
  - `true`：`zone` 在 DBM 语义下**满足**对角线约束 `φ`；
  - `false`：`zone` **不保证**满足 `φ`（或者 `φ` 不适用/非法，比如索引越界）。
  - 实现方式：
    - 取 DBM 的 `current = dbm[i,j]`，
    - 构造 `bound = (#, c)`，
    - 若 `current <= bound`（DBM 比较）则返回 `true`。

- **`diagonal_implication(from, to, G)` 的返回值**：
  - `true`：对所有 `φ ∈ G`，只要 `from` 满足 `φ`，`to` 也满足 `φ`（即对角线 guard 被“蕴含/保留”）；
  - `false`：存在某个 `φ ∈ G` 使得 `from` 满足 `φ` 而 `to` 不满足，则蕴含失败。

- **语义直观解释**：
  - `zone_entails` 是“单个 zone 是否满足一个对角线约束”。
  - `diagonal_implication` 是“从一个 zone 到另一个 zone，所有对角线 guard 是否都保持不被破坏”。

---

## TODO

1. **`LU(G)` 没有完整显式化** `已完成第一阶段`
   - `todo.tex` 里的理论形式是 `LU(G)` + diagonal implication。
   - 之前代码更接近“沿用现有 LU 机制，再额外加对角线蕴含”。
   - 当前已经完成第一阶段改造：`equal(...)` 的非对角线部分改为显式从状态约束中提取本地 `L/U`，再调用 `my_lu_plus(...)`。
   - 仍可继续优化的点：
     - 对 `L/U` 提取做缓存；
     - 更系统地检查它与原有 local/global LU map 的关系。

2. **有限性部分还停留在文档层** `已完成第一阶段`
   - `todo.tex` 已经整理了 finite simulation / strong finiteness / Boolean abstraction 的证明路线。
   - 之前代码里没有显式的状态摘要或缓存结构。
   - 当前已经完成第一阶段工程化：
     - 引入了按 `vloc + intvars` 建立的 guard summary cache；
     - 把 `G_diag(q)` 和本地 `LU(G(q))` 放进统一摘要结构。
     - 在代码注释和 `todo.tex` 中补上了实现对象与理论对象的对应关系：
       - `guard_summary.diagonals` 对应 `G^+(q)`
       - `guard_summary.L/U` 对应当前实现中的 `LU(G(q))`
   - 仍未完成的部分：
     - 还没有显式 Boolean abstraction 类；
     - 还没有把 finite / strongly-finite 的理论条件转成可单独检查的代码结构。

3. **验证还不够系统** `大体完成`
   - 当前主要依赖手工构造的例子。
   - 已完成：
     - regression tests
     - 对原非对角线例子的无回归检查
     - 对角线 reachable / unreachable 的系统回归
     - push/pop + diagonal reachable / unreachable 的系统回归
   - 当前新增回归测试：
      - `test/bugfixes/bug-042.sh`
      - `test/bugfixes/bug-043.sh`
      - `test/bugfixes/bug-044.sh`
      - `test/bugfixes/bug-045.sh`
      - 覆盖 `B1`、`ex_tpda_structural`、`loop_push_pop`、`loop_push_pop_match`
      - 覆盖 `Diag1`、`B2_5`、`B3_4_3`、`ex_tpda`
      - 覆盖 `state_local_lu`
      - 固定 `loop_push_pop` / `loop_push_pop_match` 在 `sim` 与 `eq` 下的节点数差异
   - 还缺的小项：
      - 如果后续需要更强论证，再补一个“差异只由 `G(q)` / 本地 `LU(G(q))` 引起”的极小专项例子

## 方案：如何解决 `LU(G)` 没有完整显式化

目标不是再“外挂一次 diagonal implication”，而是把当前控制点对应的非对角线 LU bounds 也真正做成 `G(q)` 驱动。

### 1. 先把 `G(q)` 分成两部分

- `G_diag(q)`：`G(q)` 中所有对角线约束。
- `G_nd(q)`：`G(q)` 中所有非对角线约束，也就是单钟形式：
  - `x # c`
  - 等价地看成 `x - 0 # c` 或 `0 - x # c`

现在代码里只显式保留了 `G_diag(q)`，下一步要把 `G_nd(q)` 也显式抽出来。

### 2. 从 `G_nd(q)` 提取本地 LU bounds

对每个时钟 `x`，扫描 `G_nd(q)`：

- 若出现上界约束 `x <= c` 或 `x < c`，更新 `U_q(x)`。
- 若出现下界约束 `x >= c` 或 `x > c`，更新 `L_q(x)`。

这样得到当前控制点 `q` 的本地 bounds：

```text
L_q : Clock -> Z union {-inf}
U_q : Clock -> Z union {-inf}
```

这一步才是 `LU(G)` 的显式来源。

### 3. 让 subsumption 使用 `LU(G(q))`，而不是外部默认 LU

当前 `equal(...)` 里第一步是：

```cpp
bool lu_not_equal = _zone_semantics.equal(z1, z2, subsumption);
```

这一步依赖的是已有 zone semantics，并没有显式传入 `G(q)` 导出的 bounds。

要改成两步：

- 先在当前状态 `q` 上计算 `L_q, U_q`。
- 再用一个显式的 `equal_under_local_lu(z1, z2, L_q, U_q, subsumption)` 来做非对角线部分判断。

实现上有两种路线：

1. **推荐路线**
   - 新增一个局部 LU 比较函数，直接调用底层 DBM extrapolation / LU simulation 例程；
   - 把 `L_q, U_q` 作为参数传进去；
   - 这样语义最清楚，也最贴近 `todo.tex`。

2. **兼容路线**
   - 保留 `_zone_semantics` 框架；
   - 新增一种 “state-local LU semantics” 包装器，在调用前临时注入 `L_q, U_q`；
   - 改动面可能更大，但和现有架构更统一。

### 4. 保持 diagonal implication 作为第二层

完整判定应改成：

```text
Subsumption(q, Z1, Z2) holds iff
  Z2 <=_{LU(G(q))} Z1
  and
  diag_implication(Z2, Z1, G_diag(q))
```

equivalence 则是双向版本。

这样之后，代码结构才和 `todo.tex` 的理论形式真正一致：

- 非对角线部分：由 `G(q)` 提取 `LU(G(q))`
- 对角线部分：由 `G_diag(q)` 做 implication

### 5. 建议的具体改动顺序

1. 在 `collect_diagonal_constraints` 旁边新增一个更一般的 `collect_guard_constraints(...)`，先把 guard/invariant 全部收集出来。
2. 在此基础上拆成：
   - `collect_diagonal_constraints(...)`
   - `collect_local_lu_bounds(...)`
3. 在 `equal(...)` 中先算本地 `L_q/U_q`，替换现有第一步 LU 判定。
4. 保留现有 `diagonal_implication(...)` 作为第二步。
5. 最后再补测试，检查：
   - 原非对角线例子结果不变；
   - 对角线例子仍符合预期；
   - 使用不同 `G(q)` 的位置时，LU 行为确实随状态变化。

### 6. 这样做的收益

- 理论和代码对齐，不再只是“LU + 对角线外挂”。
- 中期汇报时可以明确说：`LU(G)` 已从状态 guard 集合中显式提取。
- 后面做 finite/strongly-finite 的工程化也更自然，因为 `G(q)` 会真正成为 simulation 的统一输入。

## 进展记录

### 第一步改造：把本地 `LU(G(q))` 显式接入 `equal(...)`

本轮已经完成的代码改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增了统一的 `collect_guard_constraints(...)`：
  - 不再只收集 diagonal constraints；
  - 现在会把当前位置 invariant 和 outgoing guard 中的全部 clock constraints 收集出来。

- 在此基础上新增了 `collect_local_lu_bounds(...)`：
  - 对 simple constraints 做分类；
  - `x - 0 # c` 进入本地 `U`；
  - `0 - x # c` 进入本地 `L`（通过 `-value` 还原下界）。

- 新增了 `local_lu_equal(...)`：
  - 在比较两个 zone 前，先按当前 `vloc` 和 `intvars` 计算本地 `L/U`；
  - 然后直接调用底层 `tchecker::dbm::my_lu_plus(...)` 做非对角线部分的显式 LU 判定。

- `equal(...)` 的第一步已经从：

```cpp
_zone_semantics.equal(z1, z2, subsumption)
```

  改成：

```cpp
local_lu_equal(z1, z2, vloc1, intvars1, subsumption)
```

这意味着当前实现已经不再只是“沿用默认 LU 再外挂 diagonal implication”，而是开始显式由当前状态收集出来的 guard 集合驱动本地 LU 判定。

### 这一轮之后的状态

- `LU(G)` 已经有了第一版显式实现骨架。
- 但这还不是最终版，仍有两个需要继续收敛的点：
  1. 当前 `collect_local_lu_bounds(...)` 是直接从 VM 产生的 simple constraints 提取，属于工程上的就地实现；
  2. 还没有做缓存，也还没有验证它和原有 local/global LU map 的关系是否完全符合预期。

### 下一步

- 跑原例子和新增例子，检查有没有明显回归；
- 继续补测试，尤其是：
  - 原非对角线例子结果不变；
  - diagonal 例子仍满足预期；
  - `LU(G(q))` 的状态依赖性确实会影响 subsumption 判定。

### 第二步改造：补系统回归测试

本轮新增了一个稳定的非回归测试：

- `test/bugfixes/bug-042.sh`
- `test/bugfixes/bug-042.out-expected`

设计原则：

- 不比较运行时间和节点数，因为这些值容易波动；
- 只比较最终的空栈可达状态集合，保证测试稳定；
- 同时覆盖：
  - 原非对角线例子：`B1`
  - 对角线不可达例子：`ex_tpda_structural`
  - 无限 push/pop 不匹配例子：`loop_push_pop`
  - 无限 push/pop 匹配例子：`loop_push_pop_match`

本轮验证结果：

- `ctest --test-dir build -R '^bug-042$' --output-on-failure`
- 通过。

## 当前最优先的下一个 TODO

刚才这个“小粒度验证补充：做 `subsumption` 结果差异级别测试”已经完成。

目前已经固定下来的稳定差异是：

- `loop_push_pop`
  - `sim` 下节点数：`20`
  - `eq` 下节点数：`22`
- `loop_push_pop_match`
  - `sim` 下节点数：`22`
  - `eq` 下节点数：`24`

这说明当前测试系统里已经有了一个可重复的专项回归，用来固定“合并强度不同会改变图大小”。

现在下一个最合适的小 TODO 是：

1. 补一个更小、更干净的 diagonal-specific subsumption 例子；
2. 让例子本身直接展示：
   - `sim` 会合并；
   - `eq` 不会合并；
   - 差异尽量来自 diagonal / `G(q)`，而不是整套 push/pop 结构。

这个 TODO 现在也已经完成。

### 第九步改造：补最小 diagonal-specific subsumption 例子

本轮新增：

- `MyExamples/diag_subsumption_small.txt`
- `MyExamples/diag_subsumption_small_nodiag.txt`
- `test/bugfixes/bug-046.sh`
- `test/bugfixes/bug-046.out-expected`

其中：

- `diag_subsumption_small`
  - 先在同一控制点 `q` 上构造一个更精确的 zone：`x-y = 1`
  - 再回到 `q` 构造一个更宽的 zone：`1 <= x-y <= 2`
  - `q` 上唯一相关 guard 是 diagonal guard `x-y<=1`
- `diag_subsumption_small_nodiag`
  - 保持同样结构，只把 `q` 上 guard 换成 simple guard `x<=3`
  - 作为对照，确认分叉确实来自 diagonal guard，而不是模型骨架本身

固定下来的节点数差异是：

- `diag_subsumption_small`
  - `sim` 下节点数：`5`
  - `eq` 下节点数：`6`
- `diag_subsumption_small_nodiag`
  - `sim` 下节点数：`5`
  - `eq` 下节点数：`5`

这个最小例子比 `loop_push_pop*` 更适合作为后续讨论的局部证据，因为它：

- 不依赖 push/pop 匹配结构；
- 直接在单个控制点上展示 `sim` 的单向合并与 `eq` 的非合并；
- 还额外提供了一个去掉 diagonal guard 的对照版本。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-046$' --output-on-failure`

### 第三步改造：引入状态摘要缓存

本轮完成了 finite 部分的第一阶段工程化。

在 `include/tchecker/zg/details/zg.hh` 中新增了：

- `guard_summary_t`
  - 统一保存：
    - `diagonals`
    - `L`
    - `U`

- `summary_key(vloc, intvars)`
  - 用当前控制位置和整数变量估值构造缓存键。

- `guard_summary(...)`
  - 按缓存键查找摘要；
  - 若不存在，则调用 `build_guard_summary(...)` 构建后缓存。

- `build_guard_summary(...)`
  - 一次性扫描 invariant / outgoing guard；
  - 同时填充：
    - `G_diag(q)`
    - 本地 `LU(G(q))`

这样之后：

- `collect_diagonal_constraints(...)` 不再重复扫描 VM；
- `local_lu_equal(...)` 也直接复用同一个状态摘要；
- 当前实现已经从“每次比较都重新收集 guard”推进到了“按状态摘要缓存”。

本轮验证结果：

- `ctest --test-dir build -R '^bug-042$' --output-on-failure`
- 通过。
- 关键示例：
  - `ex_tpda_structural` 仍为 `{q0, q2, q5, q6}`
  - `loop_push_pop_match` 仍为 `{q0, q1a, q2, q3}`

### 第四步改造：补完整的小型验证套件

本轮新增了第二组回归测试：

- `test/bugfixes/bug-043.sh`
- `test/bugfixes/bug-043.out-expected`

它补充覆盖了：

- 原始非对角线例子：
  - `B2_5`
  - `B3_4_3`
- 对角线例子：
  - `Diag1`
  - `ex_tpda`

这样当前小型验证套件已经覆盖：

- 原非对角线例子不回归
- 对角线 reachable
- 对角线 unreachable
- push/pop + diagonal reachable
- push/pop + diagonal unreachable

本轮验证结果：

- `ctest --test-dir build -R '^bug-04[23]$' --output-on-failure`
- 通过。

## 当前阶段结论

到目前为止，验证系统这项大的工作已经可以认为 **基本完成**：

- 已经有稳定回归测试；
- 覆盖了最关键的语义类别；
- 可以支撑中期汇报中“原型已验证”的说法。

剩下和验证相关的工作，可以作为后续细化 TODO：

- 更细粒度的“subsumption 结果差异”专项测试

## 下一组 5 个小 TODO 规划

为了把“finite / strongly-finite / Boolean abstraction”这部分大的 TODO 继续拆细，
接下来按下面 5 个小 TODO 往前推进：

1. 引入显式的 `guard_profile` 对象
   - 把当前状态摘要显式投影成一个“忘记来源、只保留有限信息”的对象
   - 作为后续 abstraction / strongly-finite 工程化的载体

2. 给 `guard_profile` 补分量级比较接口
   - 明确区分：
     - `diagonal profile`
     - `local LU profile`
   - 为后续判断“哪一部分发生变化”做准备

3. 补一组 profile-stability 专项回归
   - 把现有 syntactic variant 例子按 profile 视角重新整理
   - 让“同 profile 不应改变结果”的论证更集中

4. 补一个“profile 仅因 intvars 改变而变化”的更细例子/回归
   - 从目前的 `state_local_lu*` 出发
   - 更直接地服务于 strongly-finite 方向的解释

5. 用 `guard_profile` 作为 finite / strongly-finite 后续代码入口
   - 至少把代码结构整理到“后续可在 profile 层继续扩展”
   - 即使这一轮不直接完成完整 Boolean abstraction 类，也先把接口立起来

### 第十步改造：把状态摘要缓存对 `intvars` 的依赖固定成双模式回归

本轮没有改算法行为，重点是把一个容易被后续重构误伤的语义点固定下来：

- `guard_summary` 的缓存键不仅依赖 `vloc`
- 也必须依赖当前整数变量估值 `intvars`

原因是同一个控制点 `q` 在不同整数变量估值下，可能启用不同 guard，因此本地
`LU(G(q))` 也可能不同。`MyExamples/state_local_lu.txt` 正是在固定这个语义点。

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 的 `summary_key(...)` 上补注释，明确
  `intvars` 进入缓存键是语义要求，不是实现细节
- 扩展 `test/bugfixes/bug-044.sh`
  - 原来只检查 `sim`
  - 现在同时检查 `eq`

固定结果：

- `state_local_lu`
  - `sim` 的可达状态集合：`{q0, set5, q, qerr, set1}`
- `state_local_lu_eq`
  - `eq` 的可达状态集合：`{q0, set5, q, qerr, set1}`

这一步的作用不是新增功能，而是把“状态摘要缓存必须区分整数变量估值”也做成正式回归。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-044$' --output-on-failure`

### 第十一步改造：规范化 `G^+(q)`，去掉重复 diagonal 约束

本轮完成了一个很小但对后续 finite / Boolean abstraction 工程化有帮助的步骤：

- 在 `include/tchecker/zg/details/zg.hh` 中新增
  `normalize_diagonal_constraints(...)`
- 在 `build_guard_summary(...)` 完成收集后，对 `summary.diagonals` 做：
  - 按 `(id1, id2, comparator, value)` 排序
  - 去重

这样之后，缓存里的 `G^+(q)` 会更稳定：

- 不会因为同一条 diagonal guard 在表达式里重复出现而重复存储
- 后续如果要把“状态摘要”进一步当作有限分类对象来处理，基础会更干净

本轮新增验证例子：

- `MyExamples/diag_subsumption_small_dupdiag.txt`
  - 与 `diag_subsumption_small` 结构相同
  - 只是把 `q -> qdone` 的 guard 写成重复合取：
    - `x-y<=1 && x-y<=1`

对应新增测试：

- `test/bugfixes/bug-047.sh`
- `test/bugfixes/bug-047.out-expected`

固定结果：

- `diag_subsumption_small`
  - `sim` 下节点数：`5`
  - `eq` 下节点数：`6`
- `diag_subsumption_small_dupdiag`
  - `sim` 下节点数：`5`
  - `eq` 下节点数：`6`

这一步的意义是把“重复 diagonal conjunct 不应改变摘要与合并行为”固定成正式回归。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-047$' --output-on-failure`

### 第十二步改造：固定重复 simple guard 不改变 `LU(G(q))`

本轮把与上一步对称的另一半也固定成回归：`G(q)` 里的 simple guard 即使重复，
也不应改变本地 `LU(G(q))` 摘要。

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 的 `update_guard_summary(...)` 注释中补充说明：
  - 对 simple constraints 而言，重复约束不会造成问题
  - `summary.L/U` 通过 `max` 更新保留当前最强边界

- 新增例子：
  - `MyExamples/state_local_lu_dupguards.txt`
  - 它与 `state_local_lu` 结构相同，只是把
    `x>=3 && x<=n`
    写成
    `x>=3 && x<=n && x>=3 && x<=n`

- 新增测试：
  - `test/bugfixes/bug-048.sh`
  - `test/bugfixes/bug-048.out-expected`

固定结果：

- `state_local_lu`
  - `sim`：`{q0, set5, q, qerr, set1}`
  - `eq`：`{q0, set5, q, qerr, set1}`
- `state_local_lu_dupguards`
  - `sim`：`{q0, set5, q, qerr, set1}`
  - `eq`：`{q0, set5, q, qerr, set1}`

这一步的作用是把“重复 simple conjunct 不应改变 `LU(G(q))`”也做成正式回归。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-048$' --output-on-failure`

### 第十三步改造：固定 simple guard 的书写顺序不改变 `LU(G(q))`

本轮继续把状态摘要的稳定性往前推进一小步：同一个 simple guard 集合即使
改变合取顺序，也不应改变本地 `LU(G(q))`，因此也不应改变最终可达结果。

本轮新增：

- `MyExamples/state_local_lu_reordered.txt`
  - 与 `state_local_lu` 完全同构
  - 只是把 `q -> qerr` 的 guard 从
    `x>=3 && x<=n`
    改写成
    `x<=n && x>=3`

- `test/bugfixes/bug-049.sh`
- `test/bugfixes/bug-049.out-expected`

固定结果：

- `state_local_lu`
  - `sim`：`{q0, set5, q, qerr, set1}`
  - `eq`：`{q0, set5, q, qerr, set1}`
- `state_local_lu_reordered`
  - `sim`：`{q0, set5, q, qerr, set1}`
  - `eq`：`{q0, set5, q, qerr, set1}`

这一步的意义是把“`LU(G(q))` 只依赖 guard 集合本身，而不依赖 syntactic conjunct order”
也做成正式回归。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-049$' --output-on-failure`

### 第十四步改造：固定 diagonal guard 的书写顺序不改变 `G^+(q)` 行为

本轮把与上一步对称的 diagonal 一侧也固定成回归：同一个 diagonal guard 集合
即使换一种写法、改变合取顺序，只要语义不变，就不应改变 `sim/eq` 的合并结果。

本轮新增：

- `MyExamples/diag_subsumption_small_reordered.txt`
  - 与 `diag_subsumption_small` 保持同一核心结构
  - 把 `q -> qdone` 的 guard 从单独的 `x-y<=1`
    改写成 `x<=3 && x-y<=1`
  - 其中 `x<=3` 在该模型里是冗余 simple guard，用来确认额外但不收紧语义的
    写法不会改变 diagonal-specific 的差异结果

- `test/bugfixes/bug-050.sh`
- `test/bugfixes/bug-050.out-expected`

固定结果：

- `diag_subsumption_small`
  - `sim` 下节点数：`5`
  - `eq` 下节点数：`6`
- `diag_subsumption_small_reordered`
  - `sim` 下节点数：`5`
  - `eq` 下节点数：`6`

这一步的作用是把“`G^+(q)` 的行为只依赖 guard 集合语义，而不依赖 conjunct 的具体书写顺序”
也做成正式回归。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-050$' --output-on-failure`

### 第十五步改造：固定被蕴含的冗余 diagonal guard 不改变合并结果

本轮继续往更语义化的一小步推进：如果在 `G^+(q)` 里额外加入一条已经被现有
diagonal guard 蕴含的冗余约束，那么 `sim/eq` 的合并结果不应改变。

本轮新增：

- `MyExamples/diag_subsumption_small_redundantdiag.txt`
  - 与 `diag_subsumption_small` 保持同一结构
  - 把 `q -> qdone` 的 guard 改写成：
    - `x-y<=2 && x-y<=1`
  - 其中 `x-y<=2` 是被 `x-y<=1` 蕴含的冗余 diagonal guard

- `test/bugfixes/bug-051.sh`
- `test/bugfixes/bug-051.out-expected`

固定结果：

- `diag_subsumption_small`
  - `sim` 下节点数：`5`
  - `eq` 下节点数：`6`
- `diag_subsumption_small_redundantdiag`
  - `sim` 下节点数：`5`
  - `eq` 下节点数：`6`

这一步的作用是把“向 `G^+(q)` 中加入被蕴含的冗余 diagonal guard，不应改变
当前 diagonal-specific 差异”也做成正式回归。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-051$' --output-on-failure`

### 第十六步改造：固定被蕴含的冗余 simple guard 不改变 `LU(G(q))`

本轮把与上一步对称的 simple 一侧也固定成回归：如果在 `G(q)` 的 simple guard
部分额外加入一条已经被现有 simple guard 蕴含的冗余约束，那么本地 `LU(G(q))`
与最终可达结果都不应改变。

本轮新增：

- `MyExamples/state_local_lu_redundantsimple.txt`
  - 与 `state_local_lu` 结构相同
  - 把 `q -> qerr` 的 guard 改写成：
    - `x>=3 && x<=n && x<=5`
  - 其中 `x<=5` 是冗余 simple guard：
    - 模型只会以 `n in {1,5}` 到达 `q`
    - 而 `x<=n` 在每条具体分支上都已经给出不强于 `x<=5` 的约束

- `test/bugfixes/bug-052.sh`
- `test/bugfixes/bug-052.out-expected`

固定结果：

- `state_local_lu`
  - `sim`：`{q0, set5, q, qerr, set1}`
  - `eq`：`{q0, set5, q, qerr, set1}`
- `state_local_lu_redundantsimple`
  - `sim`：`{q0, set5, q, qerr, set1}`
  - `eq`：`{q0, set5, q, qerr, set1}`

这一步的作用是把“向 simple guard 集合中加入被蕴含的冗余约束，不应改变
`LU(G(q))` 与当前结果”也做成正式回归。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-052$' --output-on-failure`

### 第十七步改造：引入显式 `guard_profile` 对象

本轮开始进入上一节新拆出来的 5 个小 TODO。

先完成第 1 个小 TODO：把当前状态摘要显式投影成一个“忘记来源、只保留有限信息”
的对象，作为后续 finite / strongly-finite / Boolean abstraction 工程化的载体。

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增 `guard_profile_t`
  - 保留三部分有限信息：
    - `diagonals`
    - `L`
    - `U`
  - 语义上对应“从当前状态摘要提取出的有限 profile”

- 新增 `guard_profile(vloc, intvars)`：
  - 先复用已有 `guard_summary(...)`
  - 再显式投影出一个 `guard_profile_t`

这样之后，代码里已经开始把两个层次区分开来：

- `summary_key(vloc, intvars)`
  - 用于缓存具体 symbolic state 的摘要
- `guard_profile(...)`
  - 用于表达“忘记具体来源后，当前状态落在哪个有限 profile 上”

这一步暂时没有改变算法行为，但它把后续 4 个小 TODO 的代码入口先整理出来了。

### 第十八步改造：给 `guard_profile` 补分量级比较接口

本轮完成上一节 5 个小 TODO 里的第 2 个：

- 不急着直接把 `guard_profile` 接进算法
- 先把它做成“可比较的对象”
- 明确区分 profile 的不同分量

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增一组比较接口：
  - `same_constraint(...)`
  - `same_diagonal_profile(...)`
  - `same_lower_profile(...)`
  - `same_upper_profile(...)`
  - `same_lu_profile(...)`
  - `same_guard_profile(...)`

这样之后，代码层面已经能显式表达：

- 两个状态摘要的 diagonal 部分是否相同
- 两个状态摘要的 local LU 部分是否相同
- 整个 guard profile 是否相同

这一步的价值在于为下一步做 profile-stability 专项回归准备接口，也方便后面把
“是哪一部分发生变化”从代码结构上拆开。

本轮还顺手让 `normalize_diagonal_constraints(...)` 复用了
`same_constraint(...)`，避免约束相等逻辑分散。

### 第十九步改造：补 `guard_profile` stability 汇总回归

本轮完成 5 个小 TODO 里的第 3 个：把前面已经有的 syntax / semantic 变体，
按 profile 视角重新整理成一个更集中的稳定性回归。

本轮新增：

- `test/bugfixes/bug-053.sh`
- `test/bugfixes/bug-053.out-expected`

这个测试分成两组：

- `diag_profile`
  - `base`
  - `dupdiag`
  - `reordered`
  - `redundant`

- `lu_profile`
  - `base`
  - `dupguards`
  - `reordered`
  - `redundant`

固定下来的结果是：

- 在 `diag_profile` 这一组里
  - 所有变体都保持：
    - `sim = 5`
    - `eq = 6`

- 在 `lu_profile` 这一组里
  - 所有变体都保持：
    - `sim = {q0, set5, q, qerr, set1}`
    - `eq = {q0, set5, q, qerr, set1}`

这一步的意义是：

- 不再只是零散地说“某两个变体结果一样”
- 而是开始按 `guard_profile` 家族来固定稳定性
- 为后面继续做“profile 变化来源”与 strongly-finite 方向的小 TODO 打基础

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-053$' --output-on-failure`

### 第二十步改造：补“profile 仅因 `intvars` 改变而变化”的细化回归

本轮完成 5 个小 TODO 里的第 4 个：把“profile 只因整数变量估值变化而变化”这件事
再拆成一对更直接的单分支例子。

本轮新增：

- `MyExamples/state_local_lu_only1.txt`
- `MyExamples/state_local_lu_only5.txt`

这两个例子：

- 控制结构相同
- 到达 `q` 后使用的 guard 形状相同：
  - `x>=3 && x<=n`
- 区别只在于进入 `q` 前对整数变量 `n` 的赋值不同：
  - 一个固定为 `n=1`
  - 一个固定为 `n=5`

因此它们非常直接地展示了：

- profile 的 simple / LU 部分会因 `intvars` 不同而变化
- 而这种变化会进一步影响最终可达结果

对应新增测试：

- `test/bugfixes/bug-054.sh`
- `test/bugfixes/bug-054.out-expected`

固定结果：

- `state_local_lu_only1`
  - `sim`：`{q0, set1, q}`
  - `eq`：`{q0, set1, q}`
- `state_local_lu_only5`
  - `sim`：`{q0, set5, q, qerr}`
  - `eq`：`{q0, set5, q, qerr}`

这一步比原先的 `state_local_lu` 更适合作为 strongly-finite / profile 解释里的局部证据，
因为它把“唯一变化来源就是 `intvars` 估值”这件事压得更干净。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-054$' --output-on-failure`

### 第二十一步改造：让 `guard_profile` 成为后续 finite 代码的入口层

本轮完成这组 5 个小 TODO 里的第 5 个：

- 不直接去实现完整 Boolean abstraction 类
- 但先把现有读取路径整理到 `guard_profile` 这一层
- 让后续 finite / strongly-finite 的代码有一个更明确的入口

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增 profile 分量访问接口：
  - `diagonal_profile(...)`
  - `lower_profile(...)`
  - `upper_profile(...)`

- 把现有几个读取摘要内容的地方统一改成先取 `guard_profile(...)`：
  - `local_lu_equal(...)`
  - `collect_diagonal_constraints(...)`
  - `collect_local_lu_bounds(...)`

这样之后，代码结构已经进一步收束成：

- `guard_summary`
  - 面向缓存实现
- `guard_profile`
  - 面向后续 finite / abstraction 扩展

这一步仍然没有改变算法行为，但它把“后续从哪一层继续扩展 strongly-finite /
Boolean abstraction”这件事明确下来了。

### 第二十二步改造：引入显式 `boolean_abstraction` 对象

本轮开始进入新的大 TODO 的第 1 个小 TODO：

- 在代码里引入一个显式的 `boolean_abstraction_t`
- 先不改变当前算法行为
- 先把后续 `canonical key` / `strongly-finite` 会依赖的对象层单独立出来

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增：
  - `boolean_abstraction_t`
  - `same_boolean_abstraction(...)`
  - `boolean_abstraction(guard_profile const &)`
  - `boolean_abstraction(vloc, intvars)`

当前这层的定位是：

- `guard_summary`
  - 面向缓存实现
- `guard_profile`
  - 面向有限信息归一化
- `boolean_abstraction`
  - 面向后续 finite / strongly-finite / canonical key 工程化

在当前版本里，`boolean_abstraction_t` 还只是把 `guard_profile` 中已经归一化好的有限信息
再显式包一层：

- `diagonal_bits`
  - 当前的有限对角线信息
- `lower_bits`
  - 当前的有限下界信息
- `upper_bits`
  - 当前的有限上界信息

也就是说，这一步的目标不是“新增新语义”，而是把后续要继续扩展的对象边界提前固定下来。

这样后面继续做：

- abstraction class
- canonical key
- strongly-finite 条件接口

时，就不必再直接依赖缓存层或把 `guard_profile` 同时当作“实现对象”和“抽象对象”来混用。

### 第二十三步改造：给 `boolean_abstraction` 补 canonical key / 输出接口

本轮完成新的大 TODO 的第 2 个小 TODO：

- 给 `boolean_abstraction` 补稳定的输出表示
- 给它补 `canonical key`
- 开始让现有 finite 读取入口收束到 `boolean_abstraction` 这一层

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增：
  - `diagonal_bits(...)`
  - `lower_bits(...)`
  - `upper_bits(...)`
  - `constraint_string(...)`
  - `boolean_abstraction_string(...)`
  - `canonical_key(boolean_abstraction const &)`
  - `canonical_key(vloc, intvars)`

- 并把现有三个读取入口改成经由 `boolean_abstraction(...)`：
  - `local_lu_equal(...)`
  - `collect_diagonal_constraints(...)`
  - `collect_local_lu_bounds(...)`

这一步的意义是：

- `boolean_abstraction` 不再只是“有名字的容器”
- 它开始具备：
  - 稳定比较
  - 稳定输出
  - 稳定 key

因此它已经可以作为后续：

- abstraction-level regression
- abstraction class / canonical representative
- strongly-finite 检查入口

的直接承载对象。

当前 `canonical_key(...)` 的编码仍然是一个工程化字符串 key，
其组成是：

- `diag=[...]`
  - 对角线约束的规范序列
- `L=[...]`
  - 本地下界 profile
- `U=[...]`
  - 本地上界 profile

也就是说，这一轮依然没有改变算法行为，
但对象层次已经从：

- `guard_summary`
- `guard_profile`

推进到：

- `guard_summary`
- `guard_profile`
- `boolean_abstraction`
  - 且这一层已经有 canonical output / key

这样后面如果补 abstraction-level 回归，就可以直接固定
`boolean_abstraction_string(...)` 或 `canonical_key(...)` 的结果，
不需要再绕回缓存层做解释。

### 第二十四步改造：补 abstraction-level regression

本轮完成新的大 TODO 的第 3 个小 TODO：

- 增加 abstraction-level 回归
- 直接固定 `boolean_abstraction` 的规范输出
- 让回归不再只看最终 graph 大小或最终可达状态集合

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增 public 读取接口：
  - `abstraction_string(vloc, intvars)`
  - `abstraction_key(vloc, intvars)`

- 在 `include/tchecker/zg/details/ts.hh` 中新增 state 级包装：
  - `abstraction_string(state)`
  - `abstraction_key(state)`

- 在 `test/bugfixes/` 中新增：
  - `bug-055.cc`
  - `bug-055.out-expected`

- 在 `test/bugfixes/CMakeLists.txt` 中注册：
  - `bug-055`

`bug-055` 的测试方式不是再走 `tchecker` CLI，
而是直接在小型 C++ regression 里：

- 构造最小 `diag_profile` 家族模型
- 构造最小 `lu_profile` 家族模型
- 走 zone-graph transition system 到目标控制点
- 直接打印该点上的：
  - `abstraction_key`
  - `abstraction_string`

这一步固定下来的不是“最终探索结果”，而是 abstraction 层本身的输出。

当前固定到的现象有：

- `diag_profile`：
  - `base` 与 `dupdiag` 的 abstraction 完全一致
  - `reordered` 因为加入了 simple conjunct，`U` 分量发生变化
  - `redundant` 因为当前实现按约束语法保留 diagonal family，额外 diagonal conjunct 会出现在 abstraction 中

- `lu_profile`：
  - `base` / `dupguards` / `reordered` 在 abstraction 层保持相同的两类 `q`-state key
  - `redundant` 因为显式加入了 `x<=5`，当前 abstraction 输出中只保留 `U=[0,5]`

也就是说，这个回归开始把“哪些变化会影响 abstraction，哪些不会”
从间接行为差异提升成了直接的可观察对象。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-055$' --output-on-failure`
- `ctest --test-dir /tmp/pdta-build -R '^bug-0(53|54|55)$' --output-on-failure`

均通过。

### 第二十五步改造：补 strongly-finite 导向的小接口

本轮完成新的大 TODO 的第 4 个小 TODO：

- 增加 “same abstraction class” 的直接代码入口
- 让 strongly-finite / finite-index 相关讨论不再只停留在文档层
- 用一个最小 regression 固定“同类/异类”的基本现象

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增：
  - `same_abstraction_class(vloc1, intvars1, vloc2, intvars2)`

- 在 `include/tchecker/zg/details/ts.hh` 中新增：
  - `same_abstraction_class(state1, state2)`

- 在 `test/bugfixes/` 中新增：
  - `bug-056.cc`
  - `bug-056.out-expected`

- 在 `test/bugfixes/CMakeLists.txt` 中注册：
  - `bug-056`

`bug-056` 固定的是两个 strongly-finite 方向最关键、最小的现象：

1. `diag_subsumption_small`
   - 两个不同 zone 的 `q`-state（precise / coarse）
   - 在 abstraction 层属于同一类
   - 固定输出：
     - `same_class 1`

2. `state_local_lu`
   - 两个同名控制点 `q`，但 `intvars` 分别为 `n=1` 和 `n=5`
   - 在 abstraction 层属于不同类
   - 固定输出：
     - `same_class 0`

3. 同时还固定：
   - `q0` 与 `q(n=1)` 也不属于同一 abstraction class

这一步的意义是：

- 现在代码里已经不只是能打印 abstraction key
- 还可以直接问：
  - 两个 symbolic state 是否落在同一 abstraction class

这虽然还不是完整的 “strongly-finite 证明代码”，
但它已经把后面最需要的判断接口立起来了：

- abstraction class membership
- finite-index discussion 的最小程序化入口

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-056$' --output-on-failure`
- `ctest --test-dir /tmp/pdta-build -R '^bug-0(55|56)$' --output-on-failure`
- `ctest --test-dir /tmp/pdta-build -R '^bug-0(53|54|55|56)$' --output-on-failure`

均通过。

### 第二十六步改造：用 `boolean_abstraction` 收束现有 finite 入口

本轮完成新的大 TODO 的第 5 个、也是最后一个小 TODO：

- 把 finite 入口彻底收束到 `boolean_abstraction`
- 删掉仍残留在 `zg.hh` 内部、但已经不再承担入口职责的 `guard_profile` 辅助层
- 让当前 finite / abstraction / strongly-finite 相关读取都只经由 abstraction 层进入

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中：
  - 删除了 `guard_profile_t`
  - 删除了所有 profile 级比较/访问辅助：
    - `same_diagonal_profile(...)`
    - `same_lower_profile(...)`
    - `same_upper_profile(...)`
    - `same_lu_profile(...)`
    - `same_guard_profile(...)`
    - `diagonal_profile(...)`
    - `lower_profile(...)`
    - `upper_profile(...)`
    - `guard_profile(...)`
  - 让 `boolean_abstraction(vloc, intvars)` 直接从 `guard_summary(...)` 投影构造

这一步之后，当前代码层次正式收束为：

- `guard_summary`
  - 面向缓存实现
- `boolean_abstraction`
  - 面向 finite / abstraction / strongly-finite 工程化

也就是说，`guard_profile` 这一层已经完成了它的历史过渡任务，
不再是当前代码结构里的 active finite entry。

当前所有和这项大 TODO 直接相关的入口已经都以 `boolean_abstraction` 为中心：

- `abstraction_string(...)`
- `abstraction_key(...)`
- `same_abstraction_class(...)`
- `collect_diagonal_constraints(...)`
- `collect_local_lu_bounds(...)`
- `local_lu_equal(...)`

这样一来，这项大的工程目标就可以视为完成：

- `boolean_abstraction` 对象已经显式存在
- 有 canonical key / 输出接口
- 有 abstraction-level regression
- 有 strongly-finite 导向的 class-membership 接口
- 现有 finite 入口也已经收束到 abstraction 层

本轮验证：

- `cmake --build /tmp/pdta-build -j4`
- `ctest --test-dir /tmp/pdta-build -R '^bug-0(53|54|55|56)$' --output-on-failure`

均通过。

### 当前结论：这个大 TODO 已完成

“把 `finite / strongly-finite / Boolean abstraction`
从文档层 + 前置结构推进到代码里有显式抽象对象和可检查结构”
这一项，目前已经完成到一个清晰可汇报、可继续扩展的阶段。

当前达到的状态是：

- 理论对象在代码里已经有明确载体：
  - `guard_summary`
  - `boolean_abstraction`

- abstraction 层已经有：
  - 规范输出
  - canonical key
  - abstraction class membership 判断

- regression 不再只覆盖最终 reachability 行为，
  也开始直接覆盖 abstraction 层本身

- 后续如果继续做更强的 strongly-finite / canonical representative /
  finite-index 枚举结构，就可以在当前 abstraction 层之上继续推进，
  不需要再回头整理入口分层

### 新的大 TODO：把 abstraction 层推进到 class / registry / proof-support

在完成前一个大 TODO 之后，下一步不再是继续整理 `boolean_abstraction` 本身，
而是把它往更显式的 abstraction-class framework 推进。

这一轮先连续完成了前 3 个小 TODO：

1. 引入 `abstraction_class_t` / `abstraction_registry_t`
2. 给 registry 补最小查询接口
3. 补 registry-level regression

#### 第二十七步改造：引入显式 `abstraction_class_t` / `abstraction_registry_t`

本轮在 `include/tchecker/zg/details/zg.hh` 中新增了：

- `class_id_t`
- `abstraction_class_t`
  - `id`
  - `canonical_key`
  - `abstraction`
- `abstraction_registry_t`
  - 内部维护：
    - `canonical_key -> class_id`
    - `vector<abstraction_class_t>`

当前 registry 还没有接到主算法流程里，
它只是一个显式的 class table，不改变探索语义。

这一步的意义是：

- “同一个 abstraction class” 不再只是临时比较结果
- 现在已经可以显式存储：
  - 这个类的 id
  - 这个类的 canonical key
  - 这个类的 canonical representative abstraction

#### 第二十八步改造：给 registry 补最小查询接口

本轮继续补齐最小可用接口：

- `abstraction_registry_t::register_class(key, abstraction)`
- `abstraction_registry_t::class_id_of(key)`
- `abstraction_registry_t::class_count()`
- `abstraction_registry_t::representative(id)`

同时在 `zg/ts` 层补了 state / symbolic-state 查询入口：

- `zg_t::class_id_of(vloc, intvars, registry)`
- `ts_t::class_id_of(state, registry)`

这样后面如果要做 finite-index / class enumeration，就已经可以直接问：

- 一个状态属于哪个 class id
- 当前 registry 里一共有多少类
- 某个类的 representative 是什么

#### 第二十九步改造：补 registry-level regression

本轮新增：

- `test/bugfixes/bug-057.cc`
- `test/bugfixes/bug-057.out-expected`

并在 `test/bugfixes/CMakeLists.txt` 中注册：

- `bug-057`

`bug-057` 固定的是 registry 层最核心的几项行为：

1. `diag_subsumption_small`
   - `precise_q` / `coarse_q`
   - 得到同一个 `class_id = 0`
   - `class_count = 1`
   - representative key 固定为该 diagonal abstraction 的 canonical key

2. `state_local_lu`
   - `q(n=1)` 与 `q(n=5)`
   - 分别得到：
     - `class_id = 0`
     - `class_id = 1`
   - `class_count = 2`
   - 两个 representative key 分别固定为对应的 local-LU abstraction key

这一步之后，class / registry 这一层已经不只是“概念上存在”，
而是已经有了：

- 显式 class object
- 显式 registry
- 可查询的 class id
- 可查询的 representative
- 可回归的 class count / class membership / representative key

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-057$' --output-on-failure`
- `ctest --test-dir /tmp/pdta-build -R '^bug-0(55|56|57)$' --output-on-failure`
- `ctest --test-dir /tmp/pdta-build -R '^bug-0(53|54|55|56|57)$' --output-on-failure`

均通过。

#### 第三十步改造：补最小枚举器

本轮完成这个新大 TODO 的第 4 个小 TODO：

- 增加一个最小 offline enumerator
- 能把一组状态收集进 class registry
- 并输出：
  - 总类数
  - 每个类的 key
  - 每个类对应哪些控制点名

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增：
  - `abstraction_enumerator_t`
    - 内部维护：
      - `abstraction_registry_t`
      - `vector<set<string>>` 形式的 per-class location names
    - 提供：
      - `add(location_name, key, abstraction)`
      - `class_count()`
      - `representative(id)`
      - `locations(id)`
      - `registry()`

- 在 `zg_t` 中新增：
  - `enumerate(vloc, intvars, enumerator)`

- 在 `include/tchecker/zg/details/ts.hh` 中新增：
  - `abstraction_enumerator_t`
  - `enumerate_state(state, enumerator)`

- 在 `test/bugfixes/` 中新增：
  - `bug-058.cc`
  - `bug-058.out-expected`

- 在 `test/bugfixes/CMakeLists.txt` 中注册：
  - `bug-058`

`bug-058` 固定的输出内容就是这个最小枚举器的目标输出：

1. `diag_subsumption_small`
   - `enumerated_class_count = 1`
   - `class_0_key = ...`
   - `class_0_locations = q`

2. `state_local_lu`
   - `enumerated_class_count = 2`
   - `class_0_key = ...`
   - `class_0_locations = q`
   - `class_1_key = ...`
   - `class_1_locations = q`

注意这里的 “locations” 当前只记录控制点名字，
因此在 `state_local_lu` 这个最小例子里，两类虽然都落在控制点 `q`，
但仍然因为 abstraction key 不同而被枚举成两个不同类。

这一步的意义是：

- class registry 不再只是单点查询工具
- 现在已经可以把一批状态收集成一个有限 class family
- 并给出最小、稳定、可回归的 class-summary 输出

这正是后面继续做 strongly-finite / finite-index 证明辅助接口的直接基础。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-0(57|58)$' --output-on-failure`
- `ctest --test-dir /tmp/pdta-build -R '^bug-0(53|54|55|56|57|58)$' --output-on-failure`

均通过。

#### 第三十一步改造：补 strongly-finite / finite-index 的证明辅助接口

本轮完成这个新大 TODO 的第 5 个小 TODO：

- 把 registry / enumerator 的结果整理成 proof-support report
- 提供稳定、可回归的 report 输出
- 让“类数 / 每类 key / classes by location”不再只是散落的临时打印

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增：
  - `abstraction_class_summary_t`
  - `proof_report_t`
  - `proof_report(enumerator)`
  - `proof_report(observer)`
  - `proof_report_string(report)`
  - `proof_report_string(enumerator)`
  - `proof_report_string(observer)`

- 在 `include/tchecker/zg/details/ts.hh` 中新增：
  - `proof_report_t`
  - `proof_report(...)`
  - `proof_report_string(...)`

当前 report 的稳定输出内容包括：

- `class_count`
- 每个类的：
  - `class[id].key`
  - `class[id].locations`
- 反向视图：
  - `location[name].classes`

这一步之后，strongly-finite / finite-index 的证明辅助已经不只是
“能枚举类”，而是能形成一个结构化、稳定的 summary/report。

对应新增回归：

- `test/bugfixes/bug-059.cc`
- `test/bugfixes/bug-059.out-expected`

`bug-059` 固定的是 proof-support report 本身的输出格式和内容。

#### 第三十二步改造：补探索旁路观察层

本轮完成这个新大 TODO 的第 6 个小 TODO：

- 增加一个 lightweight observation layer
- 在探索过程中旁路记录遇到的 abstraction classes
- 不修改主算法语义

本轮改动：

- 在 `include/tchecker/zg/details/zg.hh` 中新增：
  - `abstraction_observer_t`
  - `observe(vloc, intvars, observer)`

- 在 `include/tchecker/zg/details/ts.hh` 中新增：
  - `abstraction_observer_t`
  - `observe_state(state, observer)`

observer 的设计故意很轻：

- 内部复用 `abstraction_enumerator_t`
- 只增加 “探索时顺手 observe” 的语义接口
- 后续如果要接到更真实的 exploration / benchmark 流程，也能自然复用

对应新增回归：

- `test/bugfixes/bug-060.cc`
- `test/bugfixes/bug-060.out-expected`

`bug-060` 固定的是：

- 沿着实际探索路径逐步 `observe_state(...)`
- 最终得到的 observer report 输出

这和 `bug-059` 的区别是：

- `bug-059`：手工选定状态，做 proof-support summary
- `bug-060`：沿一段探索路径旁路累计 class family

两者合起来之后，这个大 TODO 就可以认为完整完成了：

- 显式 `abstraction_class_t`
- 显式 `abstraction_registry_t`
- 最小查询接口
- registry-level regression
- 最小枚举器
- proof-support report
- exploration-side observer

也就是说，当前 abstraction layer 已经推进到：

- class
- registry
- enumeration
- proof-support
- observation

这一整套框架。

本轮验证：

- `ctest --test-dir /tmp/pdta-build -R '^bug-0(59|60)$' --output-on-failure`
- `ctest --test-dir /tmp/pdta-build -R '^bug-0(53|54|55|56|57|58|59|60)$' --output-on-failure`

均通过。

### 当前结论：这个新大 TODO 已完成

“把当前 `boolean_abstraction` 层推进到 `abstraction class / registry / enumeration / proof-support` 层”
这一项现在已经完成。

当前代码已经具备：

- abstraction object
- abstraction class
- class registry
- class id / representative 查询
- class enumeration
- proof-support report
- exploration-side observation layer

因此如果下一步继续推进，更自然的方向已经不再是“把这些对象立起来”，
而是：

- 用它们去做更强的 finite-index / strongly-finite 实验与论证
- 或者把 observer 接进更真实的 exploration / benchmark pipeline

### 第五步改造：补理论对象与实现对象的对应关系

本轮没有再改算法行为，重点是把“代码里这个摘要到底对应理论中的什么”写清楚。

已完成：

- 在 `include/tchecker/zg/details/zg.hh` 中为 `guard_summary_t`、`guard_summary(...)`、
  `build_guard_summary(...)`、`update_guard_summary(...)` 增加注释，明确：
  - `diagonals` 对应 `G^+(q)`
  - `L/U` 对应当前实现中的 `LU(G(q))`
- 在 `todo.tex` 中新增了实现对应小节，明确说明：
  - 当前实现已经按 `LU(G(q)) + diagonal implication` 的结构组织；
  - Boolean abstraction 仍然是后续工作。

这一轮之后，finite / G-simulation 这项大的工作可以认为已经完成到
“中期汇报可清楚解释”的程度。

### 第六步改造：补状态相关 `LU(G(q))` 的定向测试

本轮新增了一个最小例子：

- `MyExamples/state_local_lu.txt`

这个例子的目的不是展示对角线本身，而是展示：

- 同一个控制点 `q`
- 不同整数变量估值 `n`
- 会导致不同的 simple guard 集合
- 因而当前实现中的本地 `LU(G(q))` 会随状态变化

在这个模型里：

- 一条路径把 `n` 设为 `1`
- 另一条路径把 `n` 设为 `5`
- 最终从 `q` 到 `qerr` 的 guard 是 `x>=3 && x<=n`

因此只有 `n=5` 分支能到达 `qerr`，这把“`G(q)` 受整数变量估值影响”固定成了一个可复现模型。

对应新增测试：

- `test/bugfixes/bug-044.sh`
- `test/bugfixes/bug-044.out-expected`

本轮验证结果：

- `ctest --test-dir build -R '^bug-044$' --output-on-failure`
- 通过。

### 第七步改造：补 benchmark 对照表

本轮补了一个可直接用于汇报的小型 benchmark 对照。

选取的例子：

- 原始非对角线例子：
  - `B1`
  - `B2_5`
  - `B3_4_3`
- 新增对角线/扩展示例：
  - `ex_tpda_structural`
  - `ex_tpda`
  - `loop_push_pop`
  - `loop_push_pop_match`
  - `state_local_lu`

当前结果：

| Example | Nodes | Time (microseconds) | Reachable states |
|---|---:|---:|---|
| `B1` | 17 | 105 | `{q0, q1}` |
| `B2_5` | 9 | 39 | `{q0, q1, r1, r2, r3, r4, r5, r6, q2}` |
| `B3_4_3` | 6 | 100 | `{q1, r1, s1}` |
| `ex_tpda_structural` | 10 | 38 | `{q0, q2, q5, q6}` |
| `ex_tpda` | 8 | 48 | `{q0, q2, q5, q6, q7}` |
| `loop_push_pop` | 20 | 58 | `{q0, q1a}` |
| `loop_push_pop_match` | 22 | 60 | `{q0, q1a, q2, q3}` |
| `state_local_lu` | 5 | 30 | `{q0, set5, q, qerr, set1}` |

解释上可直接分成三类：

- 原始例子没有明显行为回归；
- 对角线 TPDA 可区分 reachable / unreachable；
- 状态相关 `LU(G(q))` 的例子可运行并得到预期可达集合。

### 第八步改造：补“subsumption 结果差异”专项测试

本轮新增：

- `test/bugfixes/bug-045.sh`
- `test/bugfixes/bug-045.out-expected`

这个测试不再只比较最终可达状态，而是固定 `sim` / `eq` 两种 graph controller 下的节点数差异：

- `loop_push_pop`
  - `sim` 期望节点数：`20`
  - `eq` 期望节点数：`22`
- `loop_push_pop_match`
  - `sim` 期望节点数：`22`
  - `eq` 期望节点数：`24`

这一步的作用是把“subsumption 强度变化会影响图大小”这件事固定成可重复回归，而不只是口头描述。

本轮验证：

- `ctest --test-dir build -R '^bug-045$' --output-on-failure`
- 通过。

这一轮之后，原先挂着的“小粒度验证补充：subsumption 结果差异测试”可以标记为完成。
