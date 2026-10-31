# Project
<!-- agent-lite v5.1. Lasting knowledge: read only the section a task needs. -->

## Overview
- **What:** a two-state Kalman filter (tilt angle, gyro bias) in portable C++. It fuses the LSM6DSL gyroscope and accelerometer into one tilt angle.
- **Why:** a portfolio project that teaches C++, Linux, Docker and CI/CD through a problem close to the owner's control and embedded background.
- **Users:** the owner, and recruiters or interviewers reading the GitHub repository.
- **Test idea:** GitHub's CI servers have no board, so the filter is tested in three stages:
  1. Simulation: generated data with a known true angle, on the PC and in CI.
  2. Replay: real sensor data recorded from the STM32 as CSV files, on the PC and in CI.
  3. Live: the filter runs on the STM32 with live sensor data.
- **Out of scope:** yaw (needs a magnetometer), multi-axis attitude, non-linear filters (EKF), RTOS, Eigen.

## Architecture
Planned layout; update it when each step creates the real files.

| Path | Purpose |
|---|---|
| `core/` | Filter and `Matrix<R, C>`; portable, no HAL, no heap |
| `sim/` | Sensor simulator with a known true angle |
| `tests/` | Unit, simulation and replay tests |
| `data/` | Recorded sensor logs (CSV) |
| `tools/` | Python plotting scripts |
| `firmware/` | CubeMX project: sensor reading, UART logging, filter on target |
| `docker/` | Image with the host and ARM toolchains |
| `.github/workflows/` | CI |

Dependency rule: `firmware/`, `sim/` and `tests/` use `core/`; `core/` depends on none of them.

## Environment
- Development OS: Windows 11 and Ubuntu (dual boot); the owner may switch at any time. Docker work runs on Ubuntu.
- Before switching OS: update `docs/WORK.md` and commit. With a separate clone on each OS, push as well.
- Repository path on Windows: `C:\Users\msmca\projects\imu-tilt-kalman`. Ubuntu uses the same folder at `/run/media/m-can-kandamar/01D92B870CC337C0/Users/msmca/projects/imu-tilt-kalman` (`ntfs3`, read-write). Docker on this mount is untested. If it causes trouble, clone to `~/projects/imu-tilt-kalman` and push before each OS switch.
- Ubuntu tools found on 2026-10-31: `g++` 15.2, `cmake` 4.2.3, `ninja` 1.13.2, `arm-none-eabi-gcc` 14.2.1, Python 3.14.4. Not found: Docker, `gh`.
- Ubuntu links `~/.agents/USER.md` and `~/.agents/profiles` to the Windows copy on C:. Ubuntu has its own signing key, `~/.ssh/id_ed25519_signing`; both systems share `C:\Users\msmca\.ssh\allowed_signers`.
- Remote: GitHub, not created yet; it starts private.
- Shared NTFS data disk: `E:`.
- Windows tools found on 2026-10-31: `g++` 16.2 (MinGW-w64), `cmake` 4.4.3, `ninja`, `gh`, `python`, STM32CubeIDE 2.2.0. Not found: Docker, WSL, `arm-none-eabi-gcc` on PATH. C: has 22.6 GB free.
- Windows disks can be mounted by Ubuntu: `powercfg /a` reports hibernation and Fast Startup unavailable, `hiberfil.sys` does not exist, and BitLocker reads as off for C: and E:. The registry value `HiberbootEnabled = 1` has no effect while hibernation is disabled. Confirmed for C: from Ubuntu on 2026-10-31.
- This repository was created on Windows, so `core.filemode=false`: git does not see executable-bit changes in this copy. Use `git update-index --chmod=+x <file>` for scripts.
- Windows keeps the hardware clock in local time. Ubuntu is set to match (`RTC in local TZ: yes`), and its clock was correct and synchronized on 2026-10-31.

## Hardware & toolchain
- Target: STM32WLE5JCI6 (Cortex-M4, UFBGA73) on a custom board; board model not recorded. No FPU: `__FPU_PRESENT` is 0 in `stm32wle5xx.h:142`.
- Sensor: LSM6DSL on the board, I2C address `0xD6` (8-bit form).
- Toolchain & build: `arm-none-eabi-gcc`, flags `-mcpu=cortex-m4 -mthumb -mfloat-abi=soft`. STM32CubeIDE was used in the earlier project; the CMake build is created in the firmware step.
- Code generator: STM32CubeMX; the new firmware project does not exist yet.
- Pins, clocks, peripherals: earlier project used I2C2 with SDA = PA15, SCL = PB15, and SWD on PA13/PA14. Clock was MSI 4 MHz at voltage Scale 2, which allows at most 16 MHz (`stm32wlxx_hal_pwr_ex.h:165`); 48 MHz, the maximum (`stm32wlxx_hal_rcc.c:192`), needs Scale 1 and a matching flash latency. No UART was configured. The device header defines USART1, USART2 and LPUART1, and no USB peripheral.
- RTOS & memory budget: no RTOS. Flash 256 KB, RAM 64 KB in total: SRAM1 32 KB + SRAM2 32 KB (`stm32wle5xx.h:772-773`). The earlier linker script's `RAM2` region has no sections placed in it, so do not count it as extra memory.
- Debug & flashing setup: ST-Link. Flashing needs the user's approval every time.

## Decisions & constraints
- 2026-10-31 · Decision: use GoogleTest v1.18.0 for PC tests only, pinned through CMake FetchContent, and never link it into firmware · Why: most-used C++ framework in JetBrains' State of C++ 2025 (31%; Catch2 7%, doctest 4%), also used by PX4 and ArduPilot, and it fits this project's C++17. Approved by the owner.
- 2026-10-31 · Decision: start with one working copy on C:, shared by both systems, and clone to the Ubuntu disk only if it causes trouble · Why: the owner's choice; no technical blocker was found on the Windows side.
- 2026-10-31 · Decision: use the board and LSM6DSL from the earlier project · Why: the owner already has them and knows the I2C setup.
- 2026-10-31 · Decision: sample at 104 Hz · Why: the earlier 26 Hz is low for tilt estimation, and 104 Hz leaves a 9.6 ms budget per sample.
- 2026-10-31 · Constraint: no hardware FPU · Learned from: `stm32wle5xx.h:142`. Use `float` and measure the filter's cycles with the DWT counter. A rough, unmeasured estimate of 15,000 cycles per step would take about 40% of the budget at 4 MHz and 3% at 48 MHz.
- 2026-10-31 · Decision: write our own `Matrix<R, C>` instead of using Eigen · Why: small matrices only, and it is the main C++ exercise.
- 2026-10-31 · Decision: the repository starts private and goes public at the last step · Why: the owner's choice.
- 2026-10-31 · Decision: license is MIT · Why: common for portfolio libraries; proposed by the agent and not objected to.

## References
- Earlier project: `C:\Users\msmca\Documents\STM32\004_MEMS_I2C` (read-only). `lsm6dsl_mems.c/.h` hold register and sensitivity definitions. Its `main.c:147` has the sensor parse call commented out, so `accelData` and `gyroData` stay 0 in the current version.
