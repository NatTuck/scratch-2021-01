
# Homework Questions

 - Javascript alert is side effect? Yea.
 - How many useState calls? However many. Optimally one, except controlled
   inputs can have their own useState hook.
 - What's a side effect.
   - That's when a function does something that has an effect that can
     be detected outside that function.
   - Any I/O (or other system call) is a side effect.
   - Mutation of shared state is a side effect.
   - Returning a value isn't a side effect.
   - console.log disallowed technically but OK usually in practice for
     debugging
 - useState *is* a side effect, but it doesn't violate the React requirement
   of pure render functions.
 - "alert" is bad style for JS in general
 - The initial value for useState can be randomly
   generated.

How do use one useState:

```
const [state, setState] = useState({
    monkeys: [{ name "Bob", weight: 50 }],
    badInput: false
});
```

# Elixir

Erlang: Telecom switches; Reliability and Concurrency (or distribution)

 - Uses "processes" as a core concept.
 - Erlang processes run concurrently, don't share memory.
 - Inter-process communication is by message passing.
 - Functional language: No side effects (except message passing and
   a couple of edge cases).
 - Side effects are accomplished by passing messages to special processes
   that can cheat (e.g. by doing system calls).
   
Elixir: Alternate language for Erlang VM. Same implementation-semantics,
different syntax.

The elixir plan for a web app:

 * A "web app" is a custom web server.
 * Each request is handled by a new Erlang process.
 * These processes can be used to handle state or to manage long-running
   connections.
 * Safe communication between processes, even across multiple physical servers,
   innately provided by the VM.

The Elixir language is basically ISL from Fundies 1.

 - It's a functional language
 - Can't mutate data
 - Can't re-assign variables
 - Linked lists are core data type
 - No loop statements
 - Loop functions: map, filter, reduce
 - Interactive REPL

Some extra features not in ISL:

 - Non-LISP syntax
 - Separate function / variable namespaces
 - Modules
 - Pattern matching
 
 - Side effects: Message passing
 - Maps (associative arrays) as a core data type
 - Lightweight processes; send/recieve 
 
## More examples

 - rw
 - tick
 
## Phoenix: Request to Response

```
request
|> build_initial_conn # (hiding in the framework)
|> router          # lib/app_web/router.ex
|> pipeline        # "
|> controller action   # lib/app_web/controllers/*
|> view             # lib/app_web/views/*
|> template         # lib/app_web/templates/*
|> send_response # (hiding in the framework)
```










 



 
 
 





