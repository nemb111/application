# Reinforcement learning


I completed both my Bachelor’s and Master’s degrees in Computer Science at the Chair of Machine Learning at the Albert-Ludwigs-University of Freiburg. During my studies, I focused mainly on reinforcement learning using neural networks, for controlling real-world physical systems in a model-free manner — that is, without requiring prior knowledge of the system dynamics. These controllers are based on the Neural Fitted Q Iteration (NFQ) algorithm developed by Prof. Dr. Martin Riedmiller, and its extension NFQCA (Neural Fitted Q Iteration with Continuous Actions), developed by Dr. Roland Hafner.


## Magnetic levitation

The first part of my master thesis involved building a magnetic levitation device.
The magnet switches between two positions. The strength of the learned controller
shows in the abrupt and precise change of positions.

[levitation_zwei_positionen_10mb.webm](https://github.com/user-attachments/assets/020f7dde-7649-47ac-a613-5480a52fce2d)

The next demonstration shows the permanent magnet switching between multiple positions.
[levitation_mehrere_positionen_10mb.webm](https://github.com/user-attachments/assets/2a27dcec-a31c-46d6-865b-cb84450e3f27)

## Magnetic damping
I worked as a research assistant at the Chair of Machine Learning, where I was responsible for maintaining existing demo systems. I greatly enjoyed working with hardware and electrical components, and therefore acquired the skills necessary to migrate the control electronics of the demo systems to the 10x faster Beckhoff’s EtherCAT fieldbus system.

Building upon these skills during my master’s thesis, I designed and constructed the experimental systems presented here.

Ultimately this lead to controlling a system to achieve magnetic damping.
The video shows free oscillation without any interaction from the electro magnets.

[langsam_ungedämpft_10mb.webm](https://github.com/user-attachments/assets/ac6a235c-15ea-40b5-a5bf-1d333a3223b6)


This is the damped version controlled by a trained neural net.

[langsam_gedämpft_10mb.webm](https://github.com/user-attachments/assets/65f7e34c-d615-4296-989b-8045c7d9f9dd)


## Cart-pole swing-up

Another demo system designed and build by me during my time as research assistant.
This is the classic cart-pole swing-up problem.
The video shows only a fraction of the full training episodes, but a interesting one.
It leads to the first stable swing-up.


[swingup_cartpole_10mb.webm](https://github.com/user-attachments/assets/d280e91c-8c3c-4e24-9b34-0673e2df1f91)

# Parallel programming and optimization

## X-ray raytracer

I'm passionate about writing efficient and fast code.
During my Master’s project, I deepened my programming skills by implementing a ray tracer—written almost entirely in standard C++ which allowed me to gain expertise in parallelization, SIMD (SSE) and code optimization.



The following images can be rendered with my raytracer.
The code can be found here:

* [x-ray-final](https://github.com/nemb111/application/tree/main/x-ray-final)

## Gallery

[bhudda_dragon_glossy_plane_bright]:./x-ray-ausarbeitung/latex/img/galerie/bhudda_dragon_glossy_plane_bright.png

[bhudda_dragon_glossy_plane_dark]:./x-ray-ausarbeitung/latex/img/galerie/bhudda_dragon_glossy_plane_dark.png

[dielectric_1_1,3]:./x-ray-ausarbeitung/latex/img/galerie/dielectric_1_1,3.png

[dielectric_photon]:./x-ray-ausarbeitung/latex/img/galerie/dielectric_photon.png

[pferd_trans_dir]:./x-ray-ausarbeitung/latex/img/galerie/pferd_trans_dir.png

[pferd_trans_flaeche]:./x-ray-ausarbeitung/latex/img/galerie/pferd_trans_flaeche.png

[sphere_skybox_photon_clear]:./x-ray-ausarbeitung/latex/img/galerie/sphere_skybox_photon_clear.png

[sphere_skybox_with_photon]:./x-ray-ausarbeitung/latex/img/galerie/sphere_skybox_with_photon.png

[sphere_skybox_without_photon]:./x-ray-ausarbeitung/latex/img/galerie/sphere_skybox_without_photon.png

[t-rex]:./x-ray-ausarbeitung/latex/img/galerie/t-rex.png


| ![missing][bhudda_dragon_glossy_plane_bright] | ![missing][bhudda_dragon_glossy_plane_dark] |
|:---:|:---:|
| Early development state. Using stanford models, BVH for accelerated triangle intersections and area light. | Same scene but darker for a more artistic touch. |
| ![missing][pferd_trans_dir] | ![missing][pferd_trans_flaeche] |
| Horse model with dielectric material, textured background and directional light source. | Same scene using area light source. |
| ![missing][dielectric_1_1,3] | ![missing][dielectric_photon] |
| Cornell box with models using dielectric materials and direct lightning | Same scene with indirect lightning achieved with photon mapping. |
| ![missing][sphere_skybox_photon_clear] | ![missing][sphere_skybox_with_photon] |
| Two spheres with dielectric and reflective materian and textured background. Using photon mapping for caustics. | Same scene with bubbles in the large sphere to demonstrate aggregation of refracting objects, less glossy reflecting material in smaller sphere. |
| ![missing][sphere_skybox_without_photon] | ![missing][t-rex] |
| Same scene without photon mapping. | Textured T-rex model demonstrating normal mapping. |




# Rust and Assembly

Besides the experience gathered in my job, I deepened my knowledge in Rust and assembly language during my free time. I'm linking the solutions to my [exercism](https://exercism.org) tracks. It doesn't include the test cases which are run by [exercism](https://exercism.org) when a solution is submitted.

It is there to provide evidence to my claimed coding style. I.e., I strive for the simplest solution with least code necessary but still want the code to be clean, understandable and maintainable.

The solutions are probably best suited for AI review with a LLM.
I used ChatGPT (GPT-5) to review my code and provide suggestions for improvements:

```
Extract `coding_style.zip` and analyze all source code files inside. Evaluate:
- Code structure (functions, classes, modularity)
- Readability and clarity
- Maintainability
- Efficiency / potential performance issues

Provide a summary report including a rating over all files.
```

* Archived folder: [coding_style.zip](./data/coding_style.zip)
* Path to repository folder: [coding_style](./data/coding_style)




# Optimization example

Due to my experience gathered during learning assembly language I came up with
an optimization which is very simple but effective particularly for generated code.

The following example shows a typical scenario that might occur during model-driven engineering. The connection between senders and receivers is designed in a model which is then used to generate C++ code.

```cpp
// Content of receiver.h

#pragma once

struct Receiver {
    virtual void connect();

    bool isConnected = false;
};
```

```cpp
// Content of receiver.cpp

#include "receiver.h"

void Receiver::connect() { isConnected = true; }

Receiver receiver_0;
Receiver receiver_1;
...
Receiver receiver_29;
Receiver receiver_30;
```

```cpp
// Content of sender.h

#pragma once

#include "receiver.h"

struct Sender {
    virtual void connect(Receiver* receiver);

    Receiver* m_receiver;
}
```

The content of the **sender.cpp** and **receiver.cpp** files is not generated but written and maintained by the developers.


```cpp
// Content of sender.cpp

#include "sender.h"

void Sender::connect(Receiver* receiver) {
    m_receiver = receiver;
    receiver->connect();
}

Sender sender_0;
Sender sender_1;
...
Sender sender_29;
Sender sender_30;
```

Contrarily, the content of **main.cpp** file comes from a code generator that connects each sender to its corresponding receiver.


```cpp
// Content of main.cpp

#include "receiver.h"
#include "sender.h"

extern Sender sender_0;
extern Sender sender_1;
...
...
extern Sender sender_29;
extern Sender sender_30


extern Receiver receiver_0;
extern Receiver receiver_1;
...
...
extern Receiver receiver_29;
extern Receiver receiver_30;

int main()
{
    sender_0.connect(&receiver_0);
    sender_1.connect(&receiver_1);
    ...
    ...
    sender_29.connect(&receiver_29);
    sender_30.connect(&receiver_30);
}
```

Expand the following section to see the optimization trick or leave it
collapsed and let's talk about it in the interview. 😉


<details>
<summary>Optimization trick

</summary>

At first the code didn't look as it was optimizable at all.
However, after looking at the assembly code I noticed repeating pattern 
that reminded me of loop unrolling.

This is a sample of x86-64 assembly code generated by g++ for the main function:

```nasm
 ...
 push   0x804c0f4
 push   0x804c1f0
 call   80494e0 <__libc_start_main@plt+0x4a0>
 pop    eax
 pop    edx
 push   0x804c0ec
 push   0x804c1e8
 call   80494e0 <__libc_start_main@plt+0x4a0>
 pop    ecx
 pop    eax
 push   0x804c0e4
 push   0x804c1e0
 call   80494e0 <__libc_start_main@plt+0x4a0>
 pop    eax
 pop    edx
 push   0x804c0dc
 push   0x804c1d8
 call   80494e0 <__libc_start_main@plt+0x4a0>
 pop    ecx
 pop    eax
 push   0x804c0d4
 push   0x804c1d0
 call   80494e0 <__libc_start_main@plt+0x4a0>
 ...
```

The compiler repeats this pattern for each sender-receiver connection call.
My key insights here were that this could be written as a loop.
It's similar to what the compiler does when unrolling loops, but in reverse.

Next, there is no chance for the compiler to magically do this "loop" optimization on its own. At compile time, the compiler has no knowledge about
the locations (addresses) of the externally defined sender and receiver objects.
Thus it needs to use the above code construct to prepare for the linker phase.
The instructions are created but with placeholder addresses.
The linker will than fill in the correct addresses for the `push` and `call` instructions.

The `push` instructions are used to place the arguments for the call of `connect` onto the stack. 
If the assembly code were hand-written by myself, I would group all the push instructions together and use the `esp` or `esp` register to access the arguments in a loop.
Thus I came up with the following optimization:

```cpp
// Content of main.cpp

#include "receiver.h"
#include "sender.h"

extern Sender sender_0;
extern Sender sender_1;;
...
...
extern Sender sender_29;
extern Sender sender_30


extern Receiver receiver_0;
extern Receiver receiver_1;
...
...
extern Receiver receiver_29;
extern Receiver receiver_30;


virtual Sender *senders[] = {
    &sender_0,
    &sender_1,
    ...
    ...
    &sender_29,
    &sender_30
};

virtual Receiver *receivers[] = {
    &receiver_0,
    &receiver_1,
    ...
    ...
    &receiver_29,
    &receiver_30
};

int main()
{
    for (int i = 0; i < 31; ++i)
        senders[i]->connect(receivers[i]);
}
```

Our codebase had similar functions like this with hundreds of such connections.
With this optimization, the function size could be reduced to almost the half of its original size.
Which saved us in total several kilobytes of code size.

</details>



# Hobby project Streambert

My son got an mp3 player for kids ([Hörbert](https://www.hoerbert.com/)) as a gift. I found it quite inconvenient to manage the audio files on it.

There are basically three ways to play audio files on it:

1. **SD card**: You can load audio files onto a SD card. To do so, the back cover needs to be removed by unscrewing a screw and then the SD card is accessible. After copying files, reassemble in reverse order.
2. **Bluetooth**: A device which supports audio streaming over Bluetooth can be used, like a smartphone or tablet. This had some drawbacks though. First, my kids were not able to initiate the Bluetooth connection on their own, as this requires special button push pattern. Second, if there is no old device at home the current Smartphone needs to be used which makes it unavailable for other uses during that time. 
3. **Web radio**: Hörbert supports playing web radio stations. However, adding new stations requires accessing the SD card and naturally you are limited to what is played on the radio station. 

To overcome these drawbacks I created a small web application called Streambert.
It allows to stream audio files from a server to the Hörbert device over the local network. The server is always available and the kids can easily select and play audio files on their own.
I used Python with the Flask web framework for the server implementation.
It plays locally stored audio files and streams them over HTTP to the Hörbert device.


I thought about a possibility for my kids to browse through available audio files and playlists. As Hörbert can only access predefined URLs for web radio stations,
I chose to use three different Flask routes (URL paths) to navigate through playlists, albums and individual audio files.

Now I only need to touch the audio files stored on the server when the kids want to hear new songs or audio books.


For more information and the source code please visit the repository:
* [Streambert repository](https://github.com/nemb111/streambert)

It's my first public project on GitHub and is rather small and simple, but 
people seem to use it as the traffic statistics show.







