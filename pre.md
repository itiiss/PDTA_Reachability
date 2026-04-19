```latex
\documentclass[aspectratio=169]{beamer}

% --- Theme / Style ---
\usetheme{Madrid}
\usecolortheme{dolphin}
\setbeamertemplate{navigation symbols}{}
\setbeamertemplate{footline}[frame number]

% --- Packages ---
\usepackage{amsmath, amssymb}
\usepackage{graphicx}
\usepackage{booktabs}

% --- Title Info ---
\title{Extending Timed PDA Reachability to Diagonal Constraints}
\subtitle{From Timed Automata to G-simulation}
\author{Your Name}
\institute{Your Institute}
\date{Midterm Presentation}

\begin{document}

% 1. Title
\begin{frame}
  \titlepage
\end{frame}

% 2. Agenda
\begin{frame}{Outline}
  \begin{itemize}
    \item Timed Automata (TA): basics
    \item From TA to Timed Pushdown Automata (TPDA)
    \item Reachability for TPDA: zone-based view
    \item Diagonal constraints: why LU extrapolation fails
    \item G-simulation: core idea and benefits
    \item Implementation sketch + examples
    \item Status and next steps
  \end{itemize}
\end{frame}

% 3. TA basics
\begin{frame}{Timed Automata: Model}
  \begin{itemize}
    \item States: \( (\ell, v) \) with location \(\ell\) and clock valuation \(v\)
    \item Transitions guarded by clock constraints, with resets
    \item Invariants restrict time elapse in locations
  \end{itemize}
  \vspace{0.5em}
  \textbf{Zone abstraction:}
  \begin{itemize}
    \item Represent sets of valuations by DBMs (zones)
    \item Use extrapolation (e.g., LU) to ensure finiteness
  \end{itemize}
\end{frame}

% 4. TA reachability
\begin{frame}{Reachability in TA}
  \begin{itemize}
    \item Forward exploration on symbolic states \((\ell, Z)\)
    \item Apply time-elapse + discrete transitions
    \item Subsumption: prune if new zone is simulated by existing one
  \end{itemize}
  \vspace{0.5em}
  \textbf{Key property:} finite exploration via extrapolation/simulation.
\end{frame}

% 5. TPDA model
\begin{frame}{Timed Pushdown Automata (TPDA)}
  \begin{itemize}
    \item TA + unbounded stack
    \item Transitions may push/pop symbols
    \item Well-nested runs matter for reachability
  \end{itemize}
  \vspace{0.5em}
  \textbf{Challenge:} infinite stack configurations.
\end{frame}

% 6. TPDA reachability idea
\begin{frame}{TPDA Reachability: High-level Idea}
  \begin{itemize}
    \item Convert TPDA reachability to symbolic exploration with summaries
    \item Use saturation / inference rules for push and pop
    \item Use simulation to ensure finiteness
  \end{itemize}
\end{frame}

% 7. Zone-based TPDA
\begin{frame}{Zone-based TPDA Exploration}
  \begin{itemize}
    \item Symbolic state: \((q, Z)\) with control state \(q\), zone \(Z\)
    \item Roots created by \textsc{Push}; local sets explored by \textsc{Internal}/\textsc{Pop}
    \item Finiteness depends on the simulation relation
  \end{itemize}
\end{frame}

% 8. Diagonal constraints
\begin{frame}{Diagonal Constraints}
  \begin{itemize}
    \item Constraints of form \(x_i - x_j \# c\)
    \item Needed for expressive timing dependencies
    \item Breaks classical LU-extrapolation soundness
  \end{itemize}
  \vspace{0.5em}
  \textbf{Problem:} LU alone no longer yields a safe simulation.
\end{frame}

% 9. Why LU fails
\begin{frame}{Why LU Extrapolation Fails with Diagonals}
  \begin{itemize}
    \item LU only tracks per-clock bounds
    \item Diagonals relate clock differences
    \item LU abstraction can merge zones that differ on diagonals
  \end{itemize}
  \vspace{0.5em}
  \textbf{Consequence:} false subsumption, loss of soundness.
\end{frame}

% 10. G(q) guard sets
\begin{frame}{State-based Guard Sets \(G(q)\)}
  \begin{itemize}
    \item Collect diagonal constraints from:
      \begin{itemize}
        \item invariants at \(q\)
        \item guards of outgoing edges from \(q\)
      \end{itemize}
    \item \(G(q)\) is finite; used to refine simulation
  \end{itemize}
\end{frame}

% 11. G-simulation definition
\begin{frame}{G-simulation (Idea)}
  \begin{equation*}
    v \sqsubseteq^{LU}_G v'
    \iff
    \underbrace{v \sqsubseteq_{LU(G)} v'}_{\text{non-diagonal}}
    \ \wedge\ 
    \underbrace{\forall \varphi \in G^+: (v \models \varphi \Rightarrow v' \models \varphi)}_{\text{diagonal}}
  \end{equation*}
  \begin{itemize}
    \item Keep LU for non-diagonals
    \item Add implication check for diagonals
  \end{itemize}
\end{frame}

% 12. Diagonal implication check
\begin{frame}{Diagonal Implication Check}
  \begin{itemize}
    \item Given zones \(Z_1, Z_2\) and diagonal set \(G(q)\)
    \item For each \(\varphi: x_i - x_j \# c\):
      \begin{itemize}
        \item If \(Z_1 \models \varphi\), require \(Z_2 \models \varphi\)
      \end{itemize}
    \item DBM check: compare \(dbm[i,j]\) with \((\#, c)\)
  \end{itemize}
\end{frame}

% 13. Termination intuition
\begin{frame}{Why This Terminates (Sketch)}
  \begin{itemize}
    \item LU part remains a finite simulation
    \item Diagonal part reduces to finitely many Boolean classes over \(G(q)\)
    \item Combined relation is still finite (G-simulation)
    \item Hence TPDA reachability exploration terminates
  \end{itemize}
\end{frame}

% 14. Implementation sketch
\begin{frame}{Implementation Sketch}
  \begin{itemize}
    \item Extract diagonal constraints with VM over invariants/guards
    \item Extend subsumption:
      \begin{itemize}
        \item run LU check first
        \item then diagonal implication over \(G(q)\)
      \end{itemize}
    \item Keep existing DBM operations; only add diagonal check
  \end{itemize}
\end{frame}

% 15. Example slide
\begin{frame}{Example (Push/Pop with Time Windows)}
  \begin{itemize}
    \item Push loop: generate \(a^n\) with time window
    \item Pop loop: generate \(b^n\) with time window
    \item Adjust windows to make final state reachable / unreachable
  \end{itemize}
\end{frame}

% 16. Status & next steps
\begin{frame}{Status and Next Steps}
  \begin{itemize}
    \item Implemented diagonal implication in subsumption
    \item Verified with TA/TPDA examples (reachable vs unreachable)
    \item Next: optimize guard collection, add caching
    \item Next: formal evaluation on benchmarks
  \end{itemize}
\end{frame}

\end{document}
```
