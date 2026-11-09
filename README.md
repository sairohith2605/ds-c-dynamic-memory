### Strategy 1: Combine Allocator & Memory Mapper to Force Serial Storage

An erroneous experimental strategy designed to force memory mapping right next to the address of the already allocated employee object. It combines `malloc()` and `mmap()` in attempt to force place the employees in a serial manner.

#### Experiment Result

<img width="928" height="369" alt="image" src="https://github.com/user-attachments/assets/6664a14c-c332-41c7-b4b2-d699f2c10812" />

### Strategy 2: Allocating Objects in Mapped Memory

This strategy explores mapping a 4096-byte region in the Virtual Address Space via `mmap()` and allocating objects inside it serially via consecutive offsets. The output matches expectation.

#### Experiment Result
<img width="928" alt="image" src="https://github.com/user-attachments/assets/4929b881-110c-4850-8466-a38dfd74ad45" />
