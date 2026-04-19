# Session Context Dump

更新时间：2026-04-16（Asia/Tokyo）

## 1. 项目是什么

- 这是一个在 `timed PDA` 上做可达性分析的程序。
- 命令行入口在 `src/tchecker1/tchecker.cc`。
- 现在主要用法是：
  - `-pdta sim <model>`
  - `-pdta eq <model>`
- `sim` / `eq` 是两种 graph controller：
  - `sim`：simulation 风格，允许更强合并
  - `eq`：equivalence 风格，合并更保守

当前这周的主线任务是：给项目加上对角线约束支持，并把相关行为固定成可重复回归。

## 2. 这周已经完成的核心算法改动

核心文件是：

- `include/tchecker/zg/details/zg.hh`

### 已完成的关键点

1. 在 `equal(...)` 中把比较拆成两层：
   - 非对角线部分：使用本地 `LU(G(q))`
   - 对角线部分：使用 `diagonal_implication(...)`

2. 已从当前状态的 invariant / outgoing guard 中收集状态相关约束：
   - diagonal 部分进入 `G^+(q)`
   - simple 部分进入本地 `L/U`

3. 已引入状态摘要缓存：
   - `guard_summary_t`
   - `summary_key(vloc, intvars)`
   - `guard_summary(...)`
   - `build_guard_summary(...)`

4. 已引入更显式的有限 profile 层：
   - `guard_profile_t`
   - `guard_profile(vloc, intvars)`
   - 比较接口：
     - `same_constraint(...)`
     - `same_diagonal_profile(...)`
     - `same_lower_profile(...)`
     - `same_upper_profile(...)`
     - `same_lu_profile(...)`
     - `same_guard_profile(...)`
   - 分量访问接口：
     - `diagonal_profile(...)`
     - `lower_profile(...)`
     - `upper_profile(...)`

5. 当前读取有限信息的几个入口已经收束到 `guard_profile` 层：
   - `local_lu_equal(...)`
   - `collect_diagonal_constraints(...)`
   - `collect_local_lu_bounds(...)`

### 当前代码层次

- `guard_summary`
  - 面向缓存实现
  - 依赖 `vloc + intvars`
- `guard_profile`
  - 面向后续 finite / abstraction / strongly-finite 扩展
  - 是“忘记来源、只保留有限信息”的对象

## 3. 这周新增/固定的测试与例子

### 已有回归主线

- `bug-042`：原例子 + 对角线例子 + push/pop 例子最终可达状态集合回归
- `bug-043`：补更多原/对角线例子
- `bug-044`：`state_local_lu`，固定 `intvars` 会影响本地 `LU(G(q))`
- `bug-045`：固定 `sim/eq` 图大小差异
- `bug-046`：最小 diagonal-specific subsumption 例子

### 这周新补的一串小回归

#### 对角线最小差异例子

- `MyExamples/diag_subsumption_small.txt`
  - 最小 diagonal-specific 例子
  - 固定结果：
    - `sim = 5`
    - `eq = 6`

- `MyExamples/diag_subsumption_small_nodiag.txt`
  - 对照版，去掉 diagonal guard
  - 固定结果：
    - `sim = 5`
    - `eq = 5`

对应测试：

- `test/bugfixes/bug-046.sh`

#### 对 `G^+(q)` 稳定性的回归

- `MyExamples/diag_subsumption_small_dupdiag.txt`
  - 重复 diagonal conjunct
- `MyExamples/diag_subsumption_small_reordered.txt`
  - 改写 guard 写法
- `MyExamples/diag_subsumption_small_redundantdiag.txt`
  - 加入被蕴含的冗余 diagonal guard

对应测试：

- `bug-047`
- `bug-050`
- `bug-051`

这些都固定为：

- `sim = 5`
- `eq = 6`

#### 对 `LU(G(q))` 稳定性的回归

- `MyExamples/state_local_lu_dupguards.txt`
  - 重复 simple conjunct
- `MyExamples/state_local_lu_reordered.txt`
  - simple conjunct 顺序变化
- `MyExamples/state_local_lu_redundantsimple.txt`
  - 加入被蕴含的冗余 simple guard

对应测试：

- `bug-048`
- `bug-049`
- `bug-052`

这些都固定为：

- `sim = {q0, set5, q, qerr, set1}`
- `eq = {q0, set5, q, qerr, set1}`

#### 按 profile 家族收束的稳定性回归

- `bug-053`
  - `diag_profile` 组：
    - `base`
    - `dupdiag`
    - `reordered`
    - `redundant`
  - `lu_profile` 组：
    - `base`
    - `dupguards`
    - `reordered`
    - `redundant`

固定结果：

- `diag_profile` 全组：
  - `sim = 5`
  - `eq = 6`
- `lu_profile` 全组：
  - `sim = {q0, set5, q, qerr, set1}`
  - `eq = {q0, set5, q, qerr, set1}`

#### “profile 仅因 intvars 改变而变化”的细化例子

- `MyExamples/state_local_lu_only1.txt`
  - 固定 `n=1`
  - 结果：
    - `sim = {q0, set1, q}`
    - `eq = {q0, set1, q}`

- `MyExamples/state_local_lu_only5.txt`
  - 固定 `n=5`
  - 结果：
    - `sim = {q0, set5, q, qerr}`
    - `eq = {q0, set5, q, qerr}`

对应测试：

- `bug-054`

## 4. 这周完成的小 TODO 链

已完成到 `sum.md` 中的“第二十一步改造”。

最近一组 5 个小 TODO 已全部完成：

1. 引入显式 `guard_profile`
2. 给 `guard_profile` 补分量级比较接口
3. 补 `guard_profile` stability 汇总回归
4. 补“profile 仅因 intvars 改变而变化”的细化回归
5. 让 `guard_profile` 成为后续 finite 代码入口层

## 5. 当前验证方式

### 本地可用验证方式

因为原仓库自带的 `build/` 目录不总是可直接增量编译，这周一直采用：

- 临时构建目录：`/tmp/pdta-build`

常用流程：

1. `cmake -S . -B /tmp/pdta-build -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5`
2. `ctest --test-dir /tmp/pdta-build -R '^bug-0xx$' --output-on-failure`

### 重要背景

这台机器原生 `bison` 太旧，直接全新编译会卡。
本周已经用过 workaround 并且当前 `/tmp/pdta-build` 是可用的。
如果换电脑后环境正常，优先直接正常 configure/build/test。

## 6. 当前文件状态

最重要的持续维护文件：

- `sum.md`
  - 这周所有改动、解释、TODO 分解都持续记录在这里
- `include/tchecker/zg/details/zg.hh`
  - 主要算法与工程化结构都在这里
- `test/bugfixes/CMakeLists.txt`
  - 已新增到 `bug-054`
- `MyExamples/`
  - 增加了一系列最小模型
- `test/bugfixes/`
  - 增加了 `bug-046` 到 `bug-054`

## 7. 当前大的 TODO 到哪了

### 已基本完成的大块

1. 对角线支持的原型接入
2. `LU(G(q)) + diagonal implication` 结构落地
3. 状态摘要缓存
4. 最小对角线专项例子
5. 一整套系统回归
6. `guard_profile` 这一层的工程化前置

### 还没做完的大 TODO

下一个大 TODO 是：

- 把 `finite / strongly-finite / Boolean abstraction`
  从“文档层 + 前置结构”推进到“代码里有显式抽象对象和可检查结构”

也就是说，下一步不再只是 `guard_profile`，而是要往真正显式的：

- `boolean_abstraction` 对象
- abstraction class / canonical key
- strongly-finite 条件的可检查代码入口

去推进。

## 8. 下一步建议

建议直接开始新的大 TODO，并先拆成 5 个小 TODO：

1. 引入显式 `boolean_abstraction` 对象
2. 给它补 canonical key / 输出接口
3. 补 abstraction-level 回归
4. 补 strongly-finite 导向的小接口
5. 用 `boolean_abstraction` 收束现有 finite 入口

如果换机后要继续，推荐第一步就做：

- 在 `zg.hh` 里把 `guard_profile` 再包一层或投影成显式 `boolean_abstraction_t`
- 先不改算法行为，只把理论对象在代码里立起来

## 9. 换机后恢复工作的最短路径

1. 打开仓库
2. 先读：
   - `session.md`（本文件）
   - `sum.md`
   - `include/tchecker/zg/details/zg.hh`
3. 看 `test/bugfixes/CMakeLists.txt` 确认测试列表到 `bug-054`
4. 跑一轮：
   - `ctest --test-dir /tmp/pdta-build -R '^bug-05[0-4]$' --output-on-failure`
   - 或在新机器上重新配置新的 build 目录后跑同类测试
5. 然后开始新大 TODO 的第 1 个小 TODO：显式 `boolean_abstraction`

## 10. 额外提醒

- `sum.md` 是目前最完整的中文工作日志。
- 这周没有在最终回复里做 git commit / push / PR。
- 当前重点不是“再补更多例子”，而是把 finite / strongly-finite 的代码结构继续实体化。

## 11. 换机后第一轮命令清单

下面这组命令按“尽快恢复工作”的顺序给出。

### A. 先进入仓库并看上下文

```bash
cd /path/to/PDTA_Reachability
ls
git status --short
sed -n '1,220p' session.md
sed -n '1,260p' sum.md
```

### B. 快速确认这周改动落点

```bash
sed -n '320,620p' include/tchecker/zg/details/zg.hh
sed -n '1,220p' test/bugfixes/CMakeLists.txt
ls MyExamples | rg 'diag_subsumption_small|state_local_lu'
ls test/bugfixes | rg 'bug-04[6-9]|bug-05[0-4]'
```

### C. 如果新机器环境正常，优先新建一个干净 build 目录

```bash
cmake -S . -B build-local -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build build-local --target tchecker -j4
```

如果这一步因为 parser 工具链问题失败，再考虑参考旧机器上的 `/tmp/pdta-build` 思路做 workaround。

### D. 先跑一轮最关键的 bugfix 回归

```bash
ctest --test-dir build-local -R '^bug-04[6-9]$|^bug-05[0-4]$' --output-on-failure
```

如果你想先缩小范围，推荐先跑：

```bash
ctest --test-dir build-local -R '^bug-05[3-4]$' --output-on-failure
```

解释：

- `bug-053`：profile-stability 汇总回归
- `bug-054`：profile 仅因 `intvars` 改变而变化

### E. 如果需要快速确认某个例子

```bash
./build-local/src/tchecker -pdta sim MyExamples/diag_subsumption_small.txt
./build-local/src/tchecker -pdta eq MyExamples/diag_subsumption_small.txt
./build-local/src/tchecker -pdta sim MyExamples/state_local_lu_only1.txt
./build-local/src/tchecker -pdta sim MyExamples/state_local_lu_only5.txt
```

### F. 继续 coding 前，建议再看一眼当前“下一步”

```bash
rg -n "下一个大 TODO|下一步建议|boolean_abstraction|strongly-finite" session.md sum.md
```

### G. 下一步 coding 入口

如果要直接续做，建议从这里开始看：

```bash
sed -n '320,620p' include/tchecker/zg/details/zg.hh
```

然后做新的大 TODO 的第 1 个小 TODO：

- 引入显式 `boolean_abstraction` 对象

建议优先原则：

1. 先立结构，不急着改算法行为
2. 先让理论对象在代码里有名字、有字段、有比较/输出接口
3. 每做完一步就补一个最小回归
