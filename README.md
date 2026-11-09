### Strategy 1: Combine Allocator & Memory Mapper to Force Serial Storage

An erroneous experimental strategy designed to force memory mapping right next to the address of the already allocated employee object. It combines `malloc()` and `mmap()` in attempt to force place the employees in a serial manner.

#### Experiment Result

<img width="956" height="657" alt="image" src="https://github.com/user-attachments/assets/6cd469fb-6a61-494b-b9c5-0bb7a21fb871" />


### Strategy 2: Allocating Objects in Mapped Memory

This strategy explores mapping a 4096-byte region in the Virtual Address Space via `mmap()` and allocating objects inside it serially via consecutive offsets. The output matches expectation.

#### Experiment Result
<img width="2090" height="1350" alt="image" src="https://github.com/user-attachments/assets/efc72490-b766-482c-a9af-fabe843cf34b" />
