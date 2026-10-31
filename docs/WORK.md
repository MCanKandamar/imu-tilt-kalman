# Work
<!-- agent-lite v5.1. State, not instructions. Target ≤ 80 lines.
Never delete the active task, its plan, step status or evidence to meet the target.
When over the target, do all of these, in order: move lasting lessons and constraints from
Dead ends to PROJECT.md › Decisions & constraints (as "Constraint:"); drop obsolete dead ends; trim
Done to the newest items (git keeps history); shorten Next to one line per item; remove answered questions. -->

## Now
Task: Build imu-tilt-kalman · branch: `main` for M1-M3, then one `feature/<name>` per step · started: not yet
The user writes the code. Each step ends with its check. Thresholds marked "target" are first guesses, to be set from data.

- [ ] M1 Repo and green CI: `.gitignore`, `.gitattributes` (LF line endings), LICENSE, CMake with a separate build folder per OS (`build/windows`, `build/linux`), one test, GitHub Actions. Check: a pushed commit shows a green CI run; a deliberately failing test turns it red.
- [ ] M2 `Matrix<R, C>`: fixed-size class template with add, multiply and transpose. Check: unit tests pass in CI; no heap use.
- [ ] M3 Simulator and filter: first a one-state filter (angle), then two states (angle, gyro bias); plot with Python. Check: on simulated data with a known gyro bias, the filter estimates the bias and beats gyro-only and accel-only angles (target: RMS error under 1°).
- [ ] M4 Docker: one image with CMake, GCC and `arm-none-eabi-gcc`, with pinned versions; CI runs inside it. Check: the same tests pass locally in the container and in CI.
- [ ] M5 Data recording: new firmware project; read LSM6DSL at 104 Hz and send CSV over UART, with a sequence number and timestamp on every line. Check: three logs in `data/` (still, known angle, vibration) with no missing or repeated sequence numbers. Needs the UART link from Open questions.
- [ ] M6 Replay tests: CI runs the filter over the recorded logs. Check: a test fails when the error passes its limit (target: stays within ±1° at a known still angle).
- [ ] M7 Filter on target: same `core/` on the STM32; CI cross-compiles it; measure cycles with DWT. Check: `built ✓` with memory use in CI; `HW verified` with the measured cycles per step. Raising the clock to 48 MHz needs approval, voltage Scale 1 and a matching flash latency.
- [ ] M8 Portfolio finish: README with plots, CI badge, tagged release; make the repository public after approval.

## Next
1. Start M1, on Windows or Ubuntu. Answer Open question 5 first.
2. On the first Ubuntu session: answer Open questions 3 and 4.
3. Answer Open questions 1 and 2 before M5.

## Done

## Dead ends

## Open questions
1. What is the UART link to the PC? · Blocks M5 · The owner remembers a USB port on the custom board, but the STM32WLE5 has no USB peripheral, so it could be a USB-UART bridge or power only. Check: connect the board to Windows and see whether a new COM port appears. If it does not, use a USB-UART adapter on free USART pins.
2. Which free pins can UART use on this board? · Blocks M5 · Needs the schematic or the board's connector list. Default meanwhile: none chosen.
3. Does the shared C: folder work from Ubuntu? · Blocks work on Ubuntu · Default, the owner's choice: use this C: folder if Ubuntu mounts it read-write. If file permissions, line endings or Docker cause trouble, clone from GitHub to `~/projects/imu-tilt-kalman` on the Ubuntu disk and push before each OS switch.
4. How does Ubuntu share `~/.agents`? · Blocks agent preferences on Ubuntu · Default: link `~/.agents/USER.md` and `~/.agents/profiles` to the Windows copy, so changes apply on both. This depends on C: being mounted. Ubuntu also needs its own signing key, added to GitHub and to `allowed_signers` on both systems.
5. Which test framework for M1? · Adding a dependency needs approval · Default: GoogleTest.
