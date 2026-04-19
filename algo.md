# 算法概述：PDTA 可达性判定

本文从研究视角介绍仓库内主要的可达性算法。工具实现了论文“Fast zone-based algorithms for reachability in pushdown timed automata”中的思想，针对推下式定时自动机（Pushdown Timed Automata, PDTA）计算可达的控制状态，要求栈操作良好嵌套并最终回到空栈。

## 输入模型

- **控制结构**：若干带时钟约束的控制状态（Locations）与切换（Transitions）组成的定时自动机。
- **栈机制**：每条边可带一个 `push` 或 `pop` 操作，符号来自有限字符集，形成推下系统。
- **钟变量**：遵循差分约束（Difference Bound Matrices, DBM），所有重置必须设为 0。
- **初始条件**：唯一初始状态与空栈；用户可选择 Simulation（默认）或 Equivalence 控制器决定图缩减策略。

## 总体流程

1. **解析并规范化系统**  
   - 将 TChecker 风格的输入转化为内部表示，检查语义错误（如不匹配的 push/pop）。
   - 对局部时钟、整型变量等做上下界分析，便于后续区域划分。

2. **符号状态表示**  
   - 每个节点由三部分组成：控制状态、整数估值、DBM 形态的 zone。
   - zone 捕获所有可能钟值，避免枚举连续时间。

3. **Timed Location Memory (TLM)**  
   - TLM 将 pushdown 结构映射为“二层图”：第一层记录所有（位置，zone）结点，第二层记录 push/pull 匹配关系。
   - 当遇到 `push a` 时，把目标结点记录到 `pushes[a]` 中；当出现匹配的 `pop a` 时，从已记录的 `push` 中生成新的二层链接，表示一段良好嵌套的路径。

4. **探索策略**  
   - 算法采用增量式图构建，每扩展一个节点就检查是否与已有节点“等价”或“被模拟”。
   - **Simulation 控制器**：只需保证新的 zone 不包含于旧 zone，即可丢弃冗余节点，速度快、图小。
   - **Equivalence 控制器**：要求 zone 精确等价，保留更多节点，用于需要精确辨别定时行为的情形。

5. **栈匹配与终止条件**  
   - 通过 TLM 维护所有 push/pop 的配对，使得探索过程总是沿着可能的良好嵌套路径前进。
   - 当栈再次变空且到达新的控制状态，就将该状态加入“可达集”；若用户指定目标状态，也可在首次命中时提前停止。

## 结果解释

运行 `tchecker -pdta <controller> <file>` 时，会输出 `[nodes, time(microseconds), states]`：

- `nodes`：构建的符号节点总数，反映图的复杂度。
- `time`：总运行时间（微秒）。
- `states`：在良好嵌套路径下可达的控制状态集合（栈最终为空）。

不同控制器下，`nodes` 与 `time` 会显著变化：Simulation 倾向于更小的图，而 Equivalence 保留更多结构信息，代价是更大的消耗。工具同时提供脚本自动验证多个实例，以及脚本化生成参数化基准（例如 `MyExamples/Scripts/B2.sh`）。这样的流程兼顾了推下结构的正确性和定时分析的效率，是 PDTA 可达性问题在实践中的一种高效解法。

## 面向 DFA/PDA 初学者的直观理解

假设你只熟悉有限自动机（DFA）和带栈的 PDA，可以把整个算法理解为“带计时器的 PDA 搜索”：

1. **把 PDTA 看成“PDA + 多个计时器”**。计时器会不停增长，我们用一个叫 zone 的结构记录所有可能的计时器取值区间，避免逐个枚举。
2. **扩展一个状态 = 读一条边**。就像 PDA 取一条转移，同时更新：
   - 控制状态（类似 DFA 的状态）；
   - 栈（`push` 或 `pop`）；
   - 计时器：该边要求的时钟约束是否满足，如果满足，就把重置为 0 的计时器重新置零。
3. **TLM 帮你记住谁和谁成对**。当你 `push a` 时，把对应的结点记到列表；遇到 `pop a` 时，从列表里取出所有之前压入 `a` 的结点，与当前结点组成“良好嵌套”的路径。
4. **控制器决定是否“记住”某个结点**。如果你在 Simulation 模式下看见一个结点的 zone 被旧结点完全覆盖，就丢掉它；Equivalence 则只有在两个 zone 完全一样时才当作重复。
5. **目标**：沿着所有可能的良好嵌套路径，把栈清空，看能到哪些控制状态。

### 伪代码框架

```pseudo
function PDTA_REACH(sys, controller):
    initialize queue with initial_node(sys)
    mark initial_node as seen
    reachable_states = {initial_node.control_state}
    while queue not empty:
        node = queue.pop()
        for edge in outgoing_edges(node):
            if edge.clock_constraints satisfied by node.zone:
                next = apply_edge(node, edge)   // 更新控制状态、zone、栈
                if stack_op is push:
                    record_push(next, edge.symbol)
                else if stack_op is pop:
                    for partner in matching_pushes(edge.symbol):
                        connect(partner, next)
                if stack empty:
                    reachable_states.add(next.control_state)
                if not dominated(next, controller):
                    mark next as seen
                    queue.push(next)
    return reachable_states

function dominated(node, controller):
    if controller == SIM:
        return exists seen_node with same control state and seen_node.zone ⊇ node.zone
    else if controller == EQ:
        return exists seen_node with same control state and seen_node.zone == node.zone
```

### 流程图（文本版）

```
┌──────────────┐
│ 读取 PDTA 输入 │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ 生成初始节点  │←─控制状态、空栈、zone=初始
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ 取出待探索节点 │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ 遍历所有出边  │
└──────┬───────┘
       │
       ▼
┌─────────────────────┐
│ 检查时钟约束 & 更新 zone │
└──────┬───────────────┘
       │满足
       ▼
┌────────────────────────┐
│ 执行 push/pop 并更新 TLM │
└──────┬─────────────────┘
       │
       ▼
┌─────────────────────┐
│ 若栈空，则记录可达状态 │
└──────┬───────────────┘
       │
       ▼
┌────────────────────────────┐
│ controller 判断是否丢弃该节点 │
└──────┬──────────────────────┘
       │否
       ▼
┌──────────────┐
│ 加入待探索队列 │
└──────┬───────┘
       │
       └──→ 回到“取出待探索节点”
```

这样就像在做一次“按时间约束扩展的 PDA 搜索”：不断取节点、沿着合法的计时和栈操作扩展，遇到重复时可根据控制策略剪枝，最终收集所有栈为空的控制状态。这个框架与普通 PDA 的差别主要是多了 zone 这种“同时管理所有时钟”的结构。

## 问答记录

**问：这个仓库和 TChecker 是什么关系？是不是在 TChecker 上做的扩展？**  
答：是的。TChecker 提供了解析系统、管理 DBM/zone、构建符号图等底层能力，而本仓库在其基础上加入了推下式结构（Timed Location Memory、栈操作解析）、PDTA 专用的探索逻辑和基准脚本。换句话说，它把 TChecker 的“平面” timed automata 引擎扩展为能处理“带栈 + 计时”的推下自动机，并针对 Simulation/Equivalence 控制器做了额外的剪枝策略与输出格式。

**问：这是不是直接 fork 某个 TChecker 版本？具体基于哪个版本，又额外做了哪些工作？**  
答：仓库把 TChecker 的源码直接 vendor 在 `include/tchecker/` 与 `src/` 等目录中，可以视作基于当时 TChecker 主干的一个快照，但并不是 Git 上的直接 fork（远程只有 `PDTA_Reachability` 仓库）。在此快照之上，作者额外实现了：
- 针对 PDTA 输入的解析器与系统构造（扩展 TChecker 的语法约束：例如必须把时钟重置为 0、禁止多进程等）。
- Timed Location Memory 及栈匹配逻辑，用来维护 push/pop 的嵌套关系。
- `tchecker -pdta` 子命令、Simulation/Equivalence 控制策略，以及相应的图输出格式。
- `MyExamples/` 与 `MyExamples/Scripts/` 中的大型基准和一键脚本。

因此可以把它看作“以 TChecker 某个版本为内核，再叠加 PDTA Reachability 功能”的衍生项目，而非简单 fork。

**问：`src/` 目录有哪些是 Vendor 的 TChecker，哪些是本仓库新增的？**  
答：绝大部分子目录（`algorithms`, `async_zg`, `clockbounds`, `dbm`, `expression`, `flat_system`, `fsm`, `graph`, `parsing`, `statement`, `system`, `ta`, `ts`, `utils`, `variables`, `vm`, `zg`, `zone` 等）都与 `include/tchecker/` 中的头文件相对应，直接来源于 TChecker。与 PDTA 专用逻辑强相关的文件主要集中在 `src/tchecker1/`（其中的 `tchecker.cc` 定义了 `tchecker -pdta` 的命令行入口、TLM 控制器、日志输出等），以及少量针对 TChecker 内核的补丁（例如对 `algorithms/explore/run.hh`、`utils/shared_objects.hh` 的修改）。因此可以理解为“在 vendor 的 TChecker 核心外包了一层 PDTA 应用代码”。

### PDTA 专有路径概览

- `src/tchecker1/`：实现 `tchecker -pdta` 命令的主程序，负责解析 PDTA 输入、构建 Timed Location Memory、调度 Simulation/Equivalence 控制器。
- `MyExamples/`：收录论文中的基准实例（`B1`~`B10` 等），以及 `sample.txt` 示范文件。
- `MyExamples/Scripts/`：用于参数化生成大规模实例的脚本（如 `B2.sh`、`B6.sh`），便于批量调节规模。
- `MyExamples/examples.sh`：一键运行全部基准的脚本，支持传入 `sim`/`eq` 控制器。
- `benchmark.md`（以及 `sim_default.log`、`eq.log` 等派生日志）：记录基准运行结果、节点数与耗时，方便复现实验。

**问：是不是说核心业务逻辑几乎都写在 `src/tchecker1/`，其他目录基本是 TChecker vendor？**  
答：可以这么理解——TChecker 的大部分模块（解析、DBM、zone、图构建、GC 等）在本仓库里保持原样，只在个别点做了小修补；而 PDTA 专用的命令行入口、Timed Location Memory 维护、栈匹配策略、日志格式等，确实都集中在 `src/tchecker1/` 中实现。外围的 `MyExamples/`、脚本和 benchmark 文档则是为了方便批量实验。因此你可以把仓库视作“`src/tchecker1/` + (TChecker 内核 + 基准数据)”的组合。

**问：运行脚本时的 `sim` / `eq` 控制器分别做什么？差别在哪里？**  
答：两者都是在控制“是否接受重复节点”的策略。`sim`（Simulation）采用较宽松的“仿真”剪枝：如果新节点的 zone 被某个旧节点完全覆盖，就直接丢弃新节点，从而减少图规模、加快运行；这对多数实验足够准确。`eq`（Equivalence）则要求 zone 精确等价才算重复，因此保留更多节点，能避免过度剪枝导致的行为遗漏，适合需要严格区分定时行为的场景。你可以从 benchmark 中看到：同样的输入在 `eq` 下节点和时间往往增长很多，反映了这种“精确 vs. 高效”的取舍。

**问：`sim` 会不会产生不准确的结果？`eq` 能保证完全精确吗？**  
答：`sim` 的仿真剪枝在定义上是 sound 的，意味着一旦某个节点被判定“被旧节点模拟”，我们就认为它的行为已经覆盖；但如果实际系统存在极细粒度的计时差异，仿真关系可能过早合并两条实际上不可互换的路径，从而漏掉一部分可达状态。`eq` 只在 zone 完全一致时才判定重复，相当于不做额外“近似”，因此可以作为“精确模式”，代价是节点和运行时间显著增加。常见做法是：先用 `sim` 快速探索，若要做严谨验证再切换到 `eq`。

**问：既然 `sim` 可能过早合并，那它的结果算“正确”吗？会带来什么影响？**  
答：在“仿真语义”下它仍然正确——也就是说，被 `sim` 剪枝掉的节点确实被某个旧节点覆盖，因而不会制造“虚假的可达状态”；但如果你的目标是枚举所有理论上可能的控制状态，`sim` 可能漏掉某些需要极精细计时/栈行为才能触达的状态。从性能角度看，`sim` 恰好是为了避免状态爆炸而生的，它通过合并行为来换取速度；只有在你需要逐一验证细节或怀疑仿真剪枝过度时，才建议切换到 `eq`。

**问：`sim`/`eq` 的实现是直接复用 TChecker 吗，还是 PDTA 仓库自己实现的？**  
答：两者都建立在 TChecker 的“图构建 + zone 管理”骨架上，但控制策略本身是本仓库针对 PDTA TLM 加的逻辑：在 `src/tchecker1/` 中，算法通过 Timed Location Memory 维护 push/pop 匹配，然后根据用户指定的控制器调用不同的节点比较函数（仿真或等价）。TChecker 原本也提供类似的“equal/subsumption”接口，但 PDTA 版本把它们整合进自己的探索流程里，并通过命令行 `-pdta sim/eq` 呈现给用户。

**问：`sim` 和 `eq` 具体是如何比较两个节点的？它们的不同点是什么？**  
答：比较的核心是“同一控制状态下的 zone 关系”。假设我们遇到一个新节点 `n_new`：
- `sim` 会在已探索节点中寻找某个 `n_old`，满足两者处于同一控制状态、栈配置兼容，并且 `n_old.zone` **包含** `n_new.zone`。一旦找到，就认为 `n_new` 被 `n_old` 仿真，直接丢弃。
- `eq` 则要求 `n_old.zone == n_new.zone`（即每个 DBM 条目都一致），只有在完全等价时才视为重复。

因此，`sim` 的比较结果是“被包含 → 冗余”，而 `eq` 只有在“完全相同”才认为冗余。这也导致了节点数、运行时间的巨大差异：`sim` 借助包含关系大幅减少状态；`eq` 更精细但代价更高。

**问：这些比较是基于“编码栈后的状态”吗？具体比较什么内容？**  
答：是的。PDTA 的符号状态可以理解为三元组 `(控制状态, 栈摘要, zone)`：栈摘要由 Timed Location Memory 维护，相当于把 push/pop 匹配结果编码进一个“二层图”。`sim`/`eq` 在比较时只会拿那些“第一层信息一致”（即相同控制状态、栈摘要兼容）的节点来对比 zone；否则，即使 zone 包含也不能互相替代，因为栈上下文不同。换言之，只有在“编码后的状态 `qS` 相同或兼容”时，才进一步对其 zone 做包含/等价判断。

**问：算法是完备的吗？对于不可达的例子能保证最终停止吗？**  
答：在选择 `eq` 控制器时，算法会准确枚举所有可能的符号状态（带 zone 的 `qS`），只要状态空间有限就能穷尽，从而给出完备结论。`sim` 在理论上是 sound 但不是完备的：它可能提前合并而漏掉某些路径，不过对于“不可达”判断仍然会终止（因为它探索的状态集合是 `eq` 的真子集）。至于“是否停止”，取决于 PDTA 的符号状态空间是否有限——对于带时钟、栈的系统来说，合理的 zone 约束和 TLM 限制通常能使状态空间有限，因此例子（包括不可达的）在实践中都会停止；如果构造了一个无限扩张的 PDTA，`eq`/`sim` 都会和普通 timed pushdown 一样持续生成新状态。

**问：状态空间什么时候有限？如果是像 `a^n b^n` 这种会无限 `pop` 的 PDA，它还有限吗？**  
答：有限性的关键在于“符号状态 + zone + 栈摘要”能否重复出现。对于标准的 `a^n b^n` PDA，只要栈符号集有限且 TLM 只允许良好嵌套，符号状态在“读完所有 `a` 再读 `b` 并匹配”后会回到之前的配置（zone 也会重复），因而状态空间仍然有限。只有当系统不断引入新的计时/栈组合、从未回到等价摘要时，状态空间才会无限增长。换言之，是否有限取决于 PDTA 是否具备“重复模式”：如果有周期性（例如 push/pop 成对出现、zone 受限），探索最终会停；若故意让栈操作或时钟值不断产生全新组合，就会无限扩展。

**问：能举一个会无限扩展的最小示例吗？**  
答：考虑一个只有一个时钟 `x`、两个控制状态 `q0`、`q1` 的 PDTA：  
- 在 `q0` 上有一条边 `q0 --(x>=0; push a; x:=0)--> q0`，表示每次都压入一个新符号并重置时钟。  
- 在 `q0` 到 `q1` 有一条边 `q0 --(x>=n; pop a)--> q1`（`n` 持续增大），意味着必须等待越来越长的时间才能匹配。  
如果 `n` 每次都会增加（例如随着栈深度累加），那么 zone 会记录越来越大的上界，栈摘要也不断增长，状态永远不会重复。这样的构造会让 `eq`/`sim` 都不停地产生新符号状态，因而无法在有限步内停止。

> 你可以在根目录看到两个对应的示例：  
> - `counter1.tck`：只有 push、没有 pop，栈深度无限增加。  
> - `counter2.tck`：每次 push 都会自增一个整型变量 `delay`，下一次 pop 需要更长等待时间，因而不断产生新的 zone 组合。  
> 它们与 `nest.tck`、`not_nest.tck` 位于同一层级，可直接用 `tchecker -pdta` 运行观察行为。







------

push：必须用eq，两个zone需要完全一致，互相模拟

push创建的节点是 context root，这个节点将被pop用来匹配，决定pop时候合法



figure 3 中，第二次push的zone（ `y - x ≥ 2`）被第一次push的zone（ `y - x ≥ 1`）模拟，但是两者不一致

`(q0, Z0)`代表 **空栈上下文**，`(q0, y - x ≥ 1)`代表 **至少 push 过一次的上下文**，两者在时间上可覆盖，但是stack上不一致



pop：使用sim不会改变结果

从 `Z2` 能走的所有 timed 行为，从 `Z2'` 也都能走，由于pop不产生新节点，只是消除一对括号匹配，最终都会归一到最大的zone中，对结果没有影响



这样的算法如何保证 算法停止的呢，不会存在无限push节点数无限的情况吗？
