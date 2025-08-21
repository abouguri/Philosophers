# 🍝 Philosophers - Dining Philosophers Problem

<div align="center">

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Threads](https://img.shields.io/badge/Threads-FF6B6B?style=for-the-badge&logo=cpu&logoColor=white)
![1337](https://img.shields.io/badge/1337-000000?style=for-the-badge&logo=42&logoColor=white)
![42Network](https://img.shields.io/badge/42%20Network-FF6B6B?style=for-the-badge&logoColor=white)

**A multithreaded solution to the classic Dining Philosophers Problem**

*Synchronization • Race Conditions • Mutexes • Thread Management*

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen?style=flat-square)](https://github.com/abouguri/Philosophers)
[![Code Quality](https://img.shields.io/badge/code%20quality-A-brightgreen?style=flat-square)](#)
[![License](https://img.shields.io/badge/license-MIT-blue?style=flat-square)](#)

</div>

---

## 🎯 Overview

The **Philosophers** project is an implementation of Edsger Dijkstra's famous **Dining Philosophers Problem**, a classic synchronization problem in computer science. This project demonstrates advanced concepts in concurrent programming, thread synchronization, and deadlock prevention using **POSIX threads** and **mutexes**.

### 🧠 The Problem

> *Five philosophers sit around a circular table. Each philosopher alternates between thinking, eating, and sleeping. To eat, a philosopher needs two forks (one from their left, one from their right). The challenge is to design a solution where no philosopher starves and no deadlocks occur.*

## ✨ Key Features

- 🧵 **Multithreaded Architecture**: Each philosopher runs as an independent thread
- 🔒 **Mutex Synchronization**: Thread-safe fork management and state control
- ⚡ **Real-time Monitoring**: Concurrent death and completion detection
- 📊 **Precise Timing**: Microsecond-accurate time management
- 🛡️ **Deadlock Prevention**: Optimized fork acquisition strategy
- 🎛️ **Configurable Parameters**: Flexible simulation settings
- 📝 **Comprehensive Logging**: Detailed state change tracking

## 🏗️ Architecture

### Project Structure
```
philo/
├── Makefile                    # Build configuration
├── include/
│   └── philosophers.h         # Header definitions
└── src/
    ├── core/                  # Core simulation logic
    │   ├── main.c            # Entry point
    │   ├── simulation.c      # Simulation setup
    │   ├── threads.c         # Thread management
    │   ├── routine.c         # Philosopher behavior
    │   └── extra.c           # Additional utilities
    ├── data/                  # Data structures
    │   ├── forks.c           # Fork management
    │   └── meals.c           # Meal tracking
    ├── synchronization/       # Thread synchronization
    │   ├── mutex_utils.c     # Mutex operations
    │   └── philo_state.c     # State management
    └── utils/                 # Utility functions
        ├── input_validation.c # Argument validation
        ├── time_utils.c      # Time operations
        ├── time_utils2.c     # Extended time functions
        ├── memory_utils.c    # Memory management
        └── logging.c         # Output formatting
```

### Core Components

| Component | Responsibility |
|-----------|---------------|
| **Philosophers** | Individual threads representing each philosopher |
| **Forks** | Shared resources protected by mutexes |
| **Monitor Threads** | Death detection and completion monitoring |
| **State Manager** | Thread-safe state transitions |
| **Time Controller** | Precise timing and sleep operations |

## 🚀 Getting Started

### Prerequisites

- **GCC Compiler** with C99 support
- **POSIX Threads** library
- **Unix-like OS** (Linux, macOS)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/abouguri/Philosophers.git
   cd Philosophers/philo
   ```

2. **Compile the project**
   ```bash
   make
   ```

3. **Run the simulation**
   ```bash
   ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [meals_required]
   ```

## 📖 Usage

### Command Syntax
```bash
./philo <n_philos> <time_to_die> <time_to_eat> <time_to_sleep> [nb_meals]
```

### Parameters

| Parameter | Description | Range | Required |
|-----------|-------------|-------|----------|
| `n_philos` | Number of philosophers (and forks) | 1-200 | ✅ |
| `time_to_die` | Max time without eating (ms) | 60+ | ✅ |
| `time_to_eat` | Time spent eating (ms) | 60+ | ✅ |
| `time_to_sleep` | Time spent sleeping (ms) | 60+ | ✅ |
| `nb_meals` | Meals per philosopher to end simulation | 0+ | ❌ |

### Example Usage

```bash
# Classic 4 philosophers setup
./philo 4 410 200 200

# With meal limit (stops after each philosopher eats 5 times)
./philo 4 410 200 200 5

# Edge case: Single philosopher (will die - only 1 fork available)
./philo 1 800 200 200

# Stress test: Many philosophers with tight timing
./philo 100 410 200 200
```

### Output Format

Each state change is logged with precise timestamps:
```
0 1 has taken a fork
0 1 has taken a fork  
0 1 is eating
200 1 is sleeping
400 1 is thinking
401 2 died
```

## 🧪 Algorithm & Strategy

### Fork Acquisition Strategy
- **Even-numbered philosophers**: Slight delay to prevent simultaneous fork grabbing
- **Ordered acquisition**: Consistent fork ordering to prevent circular wait
- **Immediate release**: Forks released immediately after eating

### Death Detection
- **Separate monitor thread**: Continuously checks philosopher states
- **Sub-10ms detection**: Ensures death reporting within required timeframe
- **Thread-safe checking**: Uses mutexes to safely read last meal times

### State Management
```c
typedef enum e_philo_state {
    EATING = 0,     // Currently eating (has both forks)
    SLEEPING = 1,   // Sleeping after eating
    THINKING = 2,   // Thinking before trying to eat
    DEAD = 3,       // Has died from starvation
    FULL = 4,       // Has eaten required number of meals
    IDLE = 5        // Initial state
} t_state;
```

## 🛠️ Technical Details

### Thread Safety Measures
- **Mutex-protected shared resources**: All forks and shared data
- **Atomic state transitions**: State changes are mutex-protected  
- **Race condition prevention**: Careful ordering of lock acquisition
- **Clean shutdown**: Proper thread joining and resource cleanup

### Performance Optimizations
- **Minimal critical sections**: Locks held for shortest possible time
- **Efficient sleep implementation**: Custom `ft_usleep` with microsecond precision
- **Memory-efficient design**: Minimal memory allocations
- **Optimized monitoring**: Smart polling intervals to reduce CPU usage

### Error Handling
- **Comprehensive input validation**: All parameters validated
- **Resource cleanup**: Proper cleanup on any failure
- **Thread management**: Safe thread creation and termination
- **Memory management**: No memory leaks

## 📊 Testing & Validation

### Test Cases Covered
- ✅ **Normal operation**: Standard philosopher counts (2-200)
- ✅ **Edge cases**: Single philosopher, large groups
- ✅ **Timing stress**: Very short death/eat/sleep times
- ✅ **Meal limits**: Simulation termination conditions
- ✅ **Input validation**: Invalid arguments handling
- ✅ **Long running**: Extended simulation stability

### Performance Benchmarks
```bash
# Test scenarios
./philo 4 410 200 200     # Stable, no deaths
./philo 4 310 200 100     # Challenging timing
./philo 1 800 200 200     # Edge case (dies)
./philo 5 200 100 100     # Tight timing (possible death)
```

## 🎯 1337 School Requirements

This project fulfills all mandatory requirements for the 42 Network curriculum:

- ✅ Each philosopher is a separate thread
- ✅ One fork between each pair of philosophers  
- ✅ Fork states protected by mutexes
- ✅ No global variables used
- ✅ Proper argument handling and validation
- ✅ State changes logged with timestamps
- ✅ Death detection within 10ms
- ✅ No data races
- ✅ Clean compilation with `-Wall -Wextra -Werror`

## 🛡️ Memory Management

- **Zero memory leaks**: All allocated memory properly freed
- **Resource cleanup**: Mutexes destroyed, threads joined
- **Error handling**: Clean exit on any allocation failure
- **Efficient allocation**: Minimal dynamic memory usage

## 🤝 Contributing

Feel free to contribute to this project! Please ensure:
- Code follows the existing style
- All tests pass
- Memory leaks are avoided
- Thread safety is maintained

## 📚 Resources

- [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- [Mutex Synchronization](https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html)
- [1337 School (42 Network)](https://1337.ma/)
- [42 Network Global](https://42.fr/)

## 📄 License

This project is part of the 1337 School curriculum (42 Network). Feel free to study and learn from it!

---

<div align="center">

**Made with ❤️ by [abouguri](https://github.com/abouguri)**

*If you found this project helpful, please consider giving it a ⭐*

</div>