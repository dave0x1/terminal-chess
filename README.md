# TChess

A chess engine written in C, built from scratch as a learning project.

The goal of v1 is a fully rule-compliant engine with a random-legal-move opponent. Every later version adds one specific improvement (stronger search, a better interface, and so on). The guiding principle is **correctness and understanding first, optimization later**.

> **Status: work in progress.** The core rules machinery is in place and tested, but legal move generation has not yet been validated with perft, and there is no search, UCI support, or interactive interface yet. See [Roadmap](#roadmap).

## What works today

- **Board representation:** 0x88 mailbox, chosen deliberately over bitboards (a possible migration later).
- **Attack detection:** knights, king, pawns, and sliding pieces, used for check detection and castling legality.
- **Pseudo-legal move generation** for every piece type, including pawn pushes, double pushes, captures, en passant, promotion, and castling.
- **Legal move filtering:** a first version that plays each pseudo-legal move, checks that the mover's king is safe, and undoes it. Tested on hand-built positions.
- **`make_move` / `unmake_move`:** all five move types (normal/capture, castling, double pawn push, en passant, promotion including promoting captures), verified with make-then-unmake round-trip tests against full board snapshots.
- **Undo history:** a fixed-size, stack-allocated stack that stores only the irreversible state (castling rights, en passant square, halfmove counter) rather than full board copies.

## Building

Requires a C compiler and `make`.

```
make
```

Run the test harness:

```
./final
```

Clean build artifacts with `make clean`.

## Project layout

| File | Purpose |
|------|---------|
| `board.h/c` | Board struct, piece lists, castling rights, insert/remove helpers |
| `attacks.h/c` | Attack detection (`is_square_attacked` and per-piece helpers) |
| `move.h/c` | Move struct, move generation, `make_move` / `unmake_move`, move array |
| `moveArray.h/c` | MoveArray struct, move insertion |
| `moveGen.c` | Move generation |
| `utility.h/c` | Printing and string conversion helpers |
| `main.c` | Test harness |

## Design notes

A few decisions worth knowing about if you're reading the code:

- **No global state.** Every function takes a `Board*` explicitly, so multiple boards can exist at once (with a future multi-game backend in mind).
- **Castling rights are derived, not tracked.** They are recomputed from whether the king and rooks are still on their home squares, which handles "rook captured on its home square" without a special case.
- **Piece lists use swap-and-compact removal.** Move generation iterates pieces far more often than pieces are added or removed.
- **Tests are round-trip and assert based.** Hand-built minimal positions isolate one move type at a time before combining them.

## Roadmap

**Toward v1**
- [ ] Validate legal move generation with perft against the known reference counts from the [Chess Programming Wiki](https://www.chessprogramming.org/Perft_Results)
- [ ] FEN parser for loading reference positions
- [ ] Finished move-list printing
- [ ] Terminal REPL
- [ ] Random-legal-move opponent
- [ ] UCI protocol support

**Later ideas**
- Stronger search and evaluation
- Possible migration to bitboards
- A multiplayer backend built on top of the engine

## Contributing

This is a personal learning project, but feedback, bug reports, and suggestions are welcome. Please open an issue.

## Acknowledgements

Much of the background reading came from the [Chess Programming Wiki](https://www.chessprogramming.org/) (0x88, make/unmake move, move generation, perft results).

## License

MIT. See [LICENSE](LICENSE).