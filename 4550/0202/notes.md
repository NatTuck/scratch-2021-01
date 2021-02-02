
# Refactoring Hangman

 - Add a CSS framework, improve the
   layout.
 - Factor out some pieces into separate
   React components.

# Erlang

 - Designed in the late 80's for telecom switches.
 - Reliability: "5 nines": 99.999% uptime
 - Hardware failure isn't an excuse
 - The only way to survive hardware failure
   is hardware redundancy.
 - Therefore, the Erlang language needed to support
   distributed programming.
 - Need to minimize the effect of software bugs.
   - Idea: Fail fast, restart
   - Parts of the program crash
   - Other parts, supervisors, can restart them
   - Idea: Functional programming with explicit state is
     good (explicit state can survive partial crashes)
 - Concurrency, reliability, crash resistance all
   sound great for web apps.

# Elixir

 - Erlang has *awful* syntax: You can't call a user
   defined function in the condition of an "if" 
   expression.
 - But Erlang has an amazing runtime system.
 - Elixir is a language that targets the Erlang VM.
   - Kind of like Scala or Kotlin target the Java VM.

 - Framework: Phoenix



