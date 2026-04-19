# 2026-04-19 组会说明文档

这个文档整理了最近围绕以下三个例子的说明，目标是明天向教授汇报当前进展，并可在换电脑后继续补充：

- `bug-055`：`boolean_abstraction` 本身长什么样
- `bug-056`：两个状态是否属于同一个 abstraction class
- `bug-059`：proof-support report 已经能输出什么

这三个例子之所以被选出来，是因为它们正好对应当前工作的三层：

1. abstraction object
2. abstraction class membership
3. proof-support report

也就是说，用这三个例子可以把这条工作线从“抽象对象”讲到“抽象类”，再讲到“证明辅助输出”。

---

## 一、当前项目进展概述

当前这条主线已经完成了两层工作。

第一层是“算法支持对角线约束”：

- 主比较逻辑已经变成：
  - 非对角线部分：本地 `LU(G(q))`
  - 对角线部分：`diagonal_implication`
- `G(q)` 已从当前状态的 invariant 和 outgoing guard 收集
- `intvars` 对本地 `LU(G(q))` 的影响已经进入状态相关摘要

第二层是“finite / strongly-finite / finite-index 的工程化对象”：

- 现在代码里已经有：
  - `boolean_abstraction`
  - `abstraction_class`
  - `abstraction_registry`
  - `abstraction_enumerator`
  - `proof_report`
  - `abstraction_observer`
- 也就是说，不只是“能比较两个状态”，而是已经能：
  - 给状态分配 abstraction key
  - 判断是否同属一个 abstraction class
  - 给 class 分配 id
  - 统计一批状态里有多少类
  - 输出每个类的 key 和对应控制点
  - 沿探索路径旁路观察见过哪些类

当前可以概括为：

- “支持对角线约束做 reachability”已经不是原型级别
- “finite-index / strongly-finite 的 supporting evidence”也已经有了代码层面的可枚举对象和可回归输出

---

## 二、为什么选 `bug-055`、`bug-056`、`bug-059`

### `bug-055`

- 作用：固定 `boolean_abstraction` 本身的输出
- 看什么：
  - `diag=[...]`
  - `L=[...]`
  - `U=[...]`
- 说明什么：
  - diagonal 信息已经进入 abstraction key
  - local `LU(G(q))` 也进入 abstraction key
  - 对重复/重排这类语法扰动，abstraction 基本稳定

### `bug-056`

- 作用：固定“两个状态是否属于同一 abstraction class”
- 看什么：
  - `same_class 1`
  - `same_class 0`
- 说明什么：
  - `diag_subsumption_small` 里 precise/coarse 两个 `q` 状态属于同一类
  - `state_local_lu` 里 `q(n=1)` 和 `q(n=5)` 不属于同一类

### `bug-059`

- 作用：固定 proof-support report
- 看什么：
  - `class_count`
  - `class[i].key`
  - `class[i].locations`
  - `location[x].classes`
- 说明什么：
  - 已经能把 finite-index / strongly-finite 的 supporting evidence 组织成稳定报告输出

推荐这三个，是因为它们正好对应三层：

- `bug-055`：abstraction object
- `bug-056`：abstraction class membership
- `bug-059`：proof-support report

---

## 三、演示脚本（修正版：不用 `ctest`，直接看真正输出）

`ctest` 只适合验收通过与否，不适合组会展示。  
组会上应直接运行测试二进制，让它打印真正有信息量的内容。

建议顺序：

### 1. 展示 abstraction key

```bash
/tmp/pdta-build/test/bugfixes/bug-055-bin
```

你要解释：

- `bug-055` 直接打印 `boolean_abstraction` 的规范输出
- 说明 abstraction 本身已经是稳定、可回归的对象
- 可以重点看：
  - diagonal profile 的 key
  - local-LU profile 的 key

### 2. 展示 same abstraction class

```bash
/tmp/pdta-build/test/bugfixes/bug-056-bin
```

你要解释：

- `diag_subsumption_small precise_vs_coarse same_class 1`
- `state_local_lu q_n1_vs_q_n5 same_class 0`
- 这说明 class-membership 已经可以直接判断
- 也说明：
  - diagonal 例子里不同 zone 可归到同一类
  - local-LU 例子里同一控制点也可能因为 `intvars` 不同而分裂成不同类

### 3. 展示 registry

```bash
/tmp/pdta-build/test/bugfixes/bug-057-bin
```

你要解释：

- 有 `class_id`
- 有 `class_count`
- 有 `representative`
- abstraction class 已经不只是“能比较”，而是“能登记、能编号、能查代表元”

### 4. 展示 enumerator

```bash
/tmp/pdta-build/test/bugfixes/bug-058-bin
```

你要解释：

- 对一批状态已经可以枚举出有限的 class family
- 输出了：
  - 总类数
  - 每个类的 key
  - 每个类对应的控制点名

### 5. 展示 proof report

```bash
/tmp/pdta-build/test/bugfixes/bug-059-bin
```

你要解释：

- 已经可以形成组会/论文风格的 summary
- 输出包括：
  - `class_count`
  - `class[i].key`
  - `class[i].locations`
  - `location[x].classes`

### 6. 展示 observer

```bash
/tmp/pdta-build/test/bugfixes/bug-060-bin
```

你要解释：

- 不改主算法语义
- 但可以沿探索路径旁路累计见过的 abstraction classes
- 这给后续 benchmark / termination evidence 铺路
- 和 `bug-059` 的区别是：
  - `bug-059`：手工选定状态后做 summary
  - `bug-060`：沿实际路径做 observation

如果时间很紧，只演示这三个就够了：

```bash
/tmp/pdta-build/test/bugfixes/bug-055-bin
/tmp/pdta-build/test/bugfixes/bug-056-bin
/tmp/pdta-build/test/bugfixes/bug-059-bin
```

---

## 四、`bug-055` 详细说明版

### 1. `bug-055` 在验证什么

`bug-055` 不是在验证最终 reachability 结果，而是在直接验证：

- 当前状态的 `boolean_abstraction` 是什么
- 这个 abstraction 对哪些模型改动敏感
- 哪些只是语法层面的重复/重排，不应该改变 abstraction

也就是说，`bug-055` 固定的是：

- abstraction 的规范输出格式
- abstraction 对 `diag` / `L` / `U` 的响应方式

对应文件：

- [bug-055.cc](/Users/zz/Documents/ta/PDTA_Reachability/test/bugfixes/bug-055.cc)
- [bug-055.out-expected](/Users/zz/Documents/ta/PDTA_Reachability/test/bugfixes/bug-055.out-expected)

输出格式是：

```text
diag=[...]
|L=[...]
|U=[...]
```

含义：

- `diag=[...]`：当前状态收集到的 diagonal constraints
- `L=[...]`：当前状态提取到的本地下界
- `U=[...]`：当前状态提取到的本地上界

### 2. `diag_profile` 这一组模型在说什么

这一组的核心模型是：

- [diag_subsumption_small.txt](/Users/zz/Documents/ta/PDTA_Reachability/MyExamples/diag_subsumption_small.txt)

模型结构很小：

- `q0 -> qprep -> q` 先构造一个比较精确的 zone
- 然后 `q -> qcoarse -> q` 再构造一个更粗的 zone
- 关键是：在 `q` 的 outgoing guard 上有 diagonal constraint
  - `x - y <= 1`

这个模型的本意不是展示完整图搜索，而是展示：

- 在状态 `q` 上，`G^+(q)` 里确实存在 diagonal guard
- 所以这个状态的 abstraction 必须带有 diagonal 信息

### 3. 为什么 `diag_profile base` 会有这样的输出

看第一行：

```text
diag_profile base key=diag=[1,2,1,1]|L=[0,-1073741823,-1073741823]|U=[0,-1073741823,-1073741823]
```

这里最重要的是：

- `diag=[1,2,1,1]`

组会上可以直接解释成：

- 这代表 `q` 上收集到了一个 diagonal guard
- 它对应模型里的：
  - `x - y <= 1`

至于编码细节：

- `1,2` 对应两个时钟
- 后面的 `1,1` 对应比较符和常数

这个不用展开，只要说：
**它说明 diagonal guard 已经进入 abstraction key。**

而：

- `L=[0,-1073741823,-1073741823]`
- `U=[0,-1073741823,-1073741823]`

表示这里没有额外有效的 simple lower/upper bound，除了零时钟项固定为 0。

### 4. 为什么 `dupdiag` 和 `base` 一样

对应模型：

- [diag_subsumption_small_dupdiag.txt](/Users/zz/Documents/ta/PDTA_Reachability/MyExamples/diag_subsumption_small_dupdiag.txt)

区别只是 guard 写成了：

- `x-y<=1 && x-y<=1`

输出却和 `base` 一模一样：

```text
diag_profile dupdiag key=diag=[1,2,1,1]|...
```

这说明：

- 重复的 diagonal conjunct 会被规范化
- abstraction 对“纯重复”是稳定的

组会上可直接说：

> 这里说明 abstraction 已经不是简单保留原始文本，而是对重复 diagonal guard 做了规范化。

### 5. 为什么 `reordered` 会变

对应模型：

- [diag_subsumption_small_reordered.txt](/Users/zz/Documents/ta/PDTA_Reachability/MyExamples/diag_subsumption_small_reordered.txt)

它把 guard 改成：

- `x<=3 && x-y<=1`

于是输出变成：

```text
diag_profile reordered key=diag=[1,2,1,1]|L=[0,-1073741823,-1073741823]|U=[0,3,-1073741823]
```

这里要强调：

- diagonal 部分没变，还是 `x-y<=1`
- 但 simple guard `x<=3` 进入了 `U`

所以 `U` 里出现了 `3`。

这说明：

- 当前 abstraction 不是只看 diagonal
- 它同时编码：
  - diagonal family
  - local `LU(G(q))`

这是组会上很重要的一句。

### 6. 为什么 `redundantdiag` 会变

对应模型：

- [diag_subsumption_small_redundantdiag.txt](/Users/zz/Documents/ta/PDTA_Reachability/MyExamples/diag_subsumption_small_redundantdiag.txt)

guard 是：

- `x-y<=2 && x-y<=1`

于是输出里有两个 diagonal 项：

```text
diag=[1,2,1,1;1,2,1,2]
```

这个现象说明：

- 当前 abstraction 已经做了排序和去重
- 但**还没有做“逻辑冗余消除”**

也就是说：

- `x-y<=2` 虽然被 `x-y<=1` 蕴含
- 但它仍然会出现在 abstraction 里

组会上可以这样说：

> 当前 abstraction 已经是显式有限对象，但它还是语法级 canonical form，而不是最小逻辑 canonical form。

### 7. `lu_profile` 这一组模型在说什么

这一组的核心模型是：

- [state_local_lu.txt](/Users/zz/Documents/ta/PDTA_Reachability/MyExamples/state_local_lu.txt)

结构是：

- 从 `q0`
- 一条路径令 `n=1`
- 一条路径令 `n=5`
- 然后都进入同一个控制点 `q`
- 在 `q` 上 guard 是：
  - `x>=3 && x<=n`

所以这个模型展示的是：

- 虽然控制点名字都叫 `q`
- 但由于 `intvars` 不同
- 当前状态对应的本地 `LU(G(q))` 也不同

### 8. 为什么 `lu_profile base` 会输出两行

看输出：

```text
lu_profile base key=diag=[]|L=[0,3]|U=[0,1]
lu_profile base key=diag=[]|L=[0,3]|U=[0,5]
```

这是因为这个模型里实际上有两个不同的 `q` 状态：

- 一个对应 `n=1`
- 一个对应 `n=5`

所以：

- `diag=[]`：说明这个例子不依赖 diagonal guard
- `L=[0,3]`：对应 `x>=3`
- `U=[0,1]`：对应 `x<=1`
- `U=[0,5]`：对应 `x<=5`

组会上可以直接强调：

> 这说明同一个控制点 `q`，会因为整数变量估值不同而落到不同的 local-LU abstraction。

### 9. `bug-055` 组会上怎么总结

可以直接这么讲：

> `bug-055` 的作用是把当前 `boolean_abstraction` 本身固定成可回归对象。它说明两件事：  
> 第一，diagonal guard 和 local `LU(G(q))` 都已经真实进入 abstraction key；  
> 第二，abstraction 对重复/重排这类语法扰动是稳定的，但对新增 simple/diagonal 约束会产生可解释的变化。

更短的版本：

> `bug-055` 证明我们现在不只是“算法行为变了”，而是已经能直接看到导致这些行为的 abstraction 对象是什么。

---

## 五、`bug-056` 详细说明版

### 1. `bug-056` 在验证什么

`bug-056` 对应文件：

- [bug-056.cc](/Users/zz/Documents/ta/PDTA_Reachability/test/bugfixes/bug-056.cc)
- [bug-056.out-expected](/Users/zz/Documents/ta/PDTA_Reachability/test/bugfixes/bug-056.out-expected)

它验证的不是 abstraction key 本身，而是更进一步的问题：

- 两个状态是否属于同一个 abstraction class

也就是：

- same abstraction class = `1`
- different abstraction class = `0`

这是从“对象长什么样”迈向“类关系是什么”的第一步。

### 2. 第一部分：`diag_subsumption_small precise_vs_coarse same_class 1`

输出第一行是：

```text
diag_subsumption_small precise_vs_coarse same_class 1
```

这里对应的两个状态都是模型 `diag_subsumption_small` 里的 `q` 状态：

- 一个是 `precise_q`
- 一个是 `coarse_q`

也就是：

- 一条路径先构造精确 zone
- 另一条路径构造更粗 zone

虽然它们的 zone 不一样，但输出说：

- `same_class 1`

这说明：

- 当前定义的 abstraction class 不是按原始 zone 精细区分
- 而是按 abstraction object 区分
- 在这个例子里，这两个状态虽然 zone 不同，但 abstraction 相同，因此属于同一类

组会上可以说：

> 这说明 abstraction class 真正捕获的是有限抽象意义下的类，而不是 DBM 级别的原始状态。

### 3. 第二部分：`state_local_lu q_n1_vs_q_n5 same_class 0`

输出第二行是：

```text
state_local_lu q_n1_vs_q_n5 same_class 0
```

这里比较的是：

- `q(n=1)`
- `q(n=5)`

虽然它们控制点都叫 `q`，但因为：

- `n=1` 时，`U=[0,1]`
- `n=5` 时，`U=[0,5]`

所以它们落在不同 abstraction classes。

这说明：

- abstraction class 会区分同一控制点下由 `intvars` 导致的本地 LU 差异

这在组会上很值得强调，因为它说明：

- 这个 abstraction 不是只看 location name
- 它真的捕捉到了状态相关的 `LU(G(q))`

### 4. 第三部分：`state_local_lu q0_vs_q_n1 same_class 0`

输出第三行：

```text
state_local_lu q0_vs_q_n1 same_class 0
```

这个更直观：

- `q0` 和 `q(n=1)` 显然不是同一类

它主要起补充作用，说明：

- abstraction class 不会把不同控制语义的状态误合并

### 5. `bug-056` 组会上怎么总结

可以直接说：

> `bug-056` 把 abstraction class membership 这个概念直接程序化了。  
> 它说明两件事：  
> 第一，`diag_subsumption_small` 里的 precise/coarse 两个 `q` 状态虽然 zone 不同，但属于同一个 abstraction class；  
> 第二，`state_local_lu` 里同样叫 `q` 的状态，若整数变量估值不同，会落到不同 abstraction classes。

更短的版本：

> `bug-056` 证明我们现在不仅能打印 abstraction，还能直接判断“两个状态是否同类”。

---

## 六、`bug-059` 详细说明版

### 1. `bug-059` 在验证什么

对应文件：

- [bug-059.cc](/Users/zz/Documents/ta/PDTA_Reachability/test/bugfixes/bug-059.cc)
- [bug-059.out-expected](/Users/zz/Documents/ta/PDTA_Reachability/test/bugfixes/bug-059.out-expected)

它不是再看单个 abstraction，也不是只看 same_class，而是在验证：

- 已经能把一组 classes 组织成一个结构化 report

这个 report 目前包含：

- `class_count`
- `class[i].key`
- `class[i].locations`
- `location[x].classes`

所以它更像：

- 组会展示材料
- 证明辅助输出
- 小型实验报告

### 2. `diag_subsumption_small report` 为什么是这样

输出是：

```text
diag_subsumption_small report
class_count=1
class[0].key=diag=[1,2,1,1]|L=[0,-1073741823,-1073741823]|U=[0,-1073741823,-1073741823]
class[0].locations=q
location[q].classes=0
```

解释：

- 这里只枚举了我们关心的两个 `q` 状态
- 它们虽然来自不同 zone，但属于同一个 abstraction class
- 所以：
  - `class_count=1`
  - 只有一个 `class[0]`
  - 它对应控制点 `q`

`location[q].classes=0` 的意思是：

- 从 location 反向看，`q` 对应到 class 0

这个输出的意义在于：

- 不再只是“这两个状态同类”
- 而是可以汇总成一个非常清楚的 class-summary

### 3. `state_local_lu report` 为什么是这样

输出是：

```text
state_local_lu report
class_count=2
class[0].key=diag=[]|L=[0,3]|U=[0,1]
class[0].locations=q
class[1].key=diag=[]|L=[0,3]|U=[0,5]
class[1].locations=q
location[q].classes=0,1
```

这里重点要讲：

- 虽然两类都在控制点 `q`
- 但因为 abstraction key 不同
- 所以 report 里会出现两个 classes

也就是说：

- `location` 只是控制点名
- `class` 是 abstraction class
- 两者不是一一对应关系

这是组会上很值得讲的一句，因为它直接说明：

> 同一个控制点可以对应多个 abstraction classes。

这正是 finite-index / strongly-finite 讨论里很重要的一点。

### 4. `bug-059` 和 `bug-058` 有什么区别

组会上如果被问到，可以回答：

- `bug-058`：最小枚举器，重点是“能枚举”
- `bug-059`：proof report，重点是“能组织成结构化 summary”

也就是说：

- `058` 更接近内部工具
- `059` 更接近展示/论证输出

### 5. `bug-059` 组会上怎么总结

可以直接这么说：

> `bug-059` 说明我们已经能把 abstraction-class family 组织成稳定的 proof-support report。  
> 输出里既有全局类数，也有每个类的 canonical key，还能从 location 反向看到它对应哪些 classes。  
> 对于后续做 finite-index / strongly-finite 的 supporting evidence，这已经是一个比较完整的输出原型。

---

## 七、这三个例子如何串起来讲

组会上可以把三者连成一条线：

### 第一层：`bug-055`

- abstraction object 长什么样

### 第二层：`bug-056`

- 两个状态是否属于同一个 abstraction class

### 第三层：`bug-059`

- 一批 abstraction classes 如何组织成 proof-support report

一句话串起来就是：

> `bug-055` 先把 abstraction 对象本身固定下来，`bug-056` 再把 class-membership 固定下来，`bug-059` 最后把 class family 组织成 report。这样从对象、到类、到报告，这条工程链已经打通了。

---

## 八、如何解释 `ctest` 输出

如果现场有人问到为什么 `ctest` 输出看起来没什么信息，可以这么解释：

- `ctest` 输出本身几乎不提供实验内容
- 它只说明两件事：
  - 测试二进制编译成功
  - 实际输出与 `.out-expected` 完全一致

也就是说：

- `ctest` 是“验收通过”
- 真正应该展示的是对应二进制的日志输出

比如对 `bug-060` 而言：

```bash
ctest --test-dir /tmp/pdta-build -R '^bug-060$' --output-on-failure
```

只能说明：

- `bug-060` 重新编译成功
- `bug-060` 实际输出和 `bug-060.out-expected` 完全一致

真正有信息的是：

```bash
/tmp/pdta-build/test/bugfixes/bug-060-bin
```

它打印的是 observer report，本身才是你要在组会上解释的内容。

---

## 九、可直接念的总结

可以直接用下面这段作为组会结尾：

> 这周主要完成了两件事。第一，PDTA reachability 在 diagonal constraints 下的比较逻辑已经落成，当前用的是本地 `LU(G(q))` 加 `diagonal_implication` 的二段式结构。第二，我把 finite / strongly-finite 相关对象从文档概念推进到了代码里的显式 abstraction-class framework。现在已经有 `boolean_abstraction`、`abstraction_class`、`registry`、`enumerator`、`proof_report` 和 `observer`，并且这些对象都有最小例子和系统回归。也就是说，目前不仅能做 diagonal reachability，还已经能对 finite-index / strongly-finite 的 supporting evidence 做可枚举、可输出、可演示的工程化支撑。

---

## 十、后续补充区（换电脑后继续写）

下面这部分留给换电脑后继续完善：

- [ ] 补一版 5 分钟中文讲稿
- [ ] 补一版 1 页 PPT 提纲
- [ ] 补更精炼的 `bug-060` 解读（适合现场展示）
- [ ] 如果需要，补教授可能会问的问题与回答
