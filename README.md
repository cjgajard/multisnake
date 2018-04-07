# multisnake

## Dependencies

- gcc
- SDL2

## Play

```sh
make && bin/main
```

## Development

There is a `test` task in make file.
You need to tell it what to test.  Example:
```sh
make test TEST=movement DEPS=movement.o
```
That will compile (without linking) movement.c and
compile, link and run test_movement.c
