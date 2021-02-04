
BEAMS := Elixir.Fib.beam Elixir.Fact.beam Elixir.Tick.beam

all: $(BEAMS)

Elixir.Fib.beam: fib.ex
	elixirc $<

Elixir.Fact.beam: fact.ex
	elixirc $<

Elixir.Tick.beam: tick.ex
	elixirc $<

clean:
	rm -f $(BEAMS)

