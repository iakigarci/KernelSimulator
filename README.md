<div id="top"></div>

<br />
<div align="center">

  <h1 align="center">Kernel simulator</h1>

  <p align="center">
    Simulation of some kernel functions coded in C
    <br />
    <!--<br />
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template">View Demo</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Report Bug</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Request Feature</a>-->
  </p>
  <h3 align="left">Connect with me:</h3>
    <p align="left">
    <a href="https://www.linkedin.com/in/iakigarcia" target="blank"><img align="center" src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/linked-in-alt.svg" alt="iakigarci" height="30" width="40" /></a>
    <a href="https://es.stackoverflow.com/users/158274/iakigarci" target="blank"><img align="center" src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/stack-overflow.svg" alt="iakigarci" height="30" width="40" /></a>
    </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<p align="center">
    <img src="kernel_example.PNG">
</p>

The objective of this program is to simulate some of the functionalities that a kernel has to manage an operating system. For this purpose, the following functionalities have been developed

- Simulate a _Scheduler_ and a _Dispatcher_ with a defined rules
- Simulate virtual memory manager
- Simulate physical memory manager

All the functionalities have been implement using advanced eficent features, such pointers, memory allocation and multithreading. 

<p align="right">(<a href="#top">back to top</a>)</p>


### Built With

The technologies used for this project are as follows:
<p align="left">
<!-- https://devicon.dev/ -->
  
<!-- AWS <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/amazonwebservices/amazonwebservices-original.svg" width="40" height="40"/>-->
<!-- BASH <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/bash/bash-original.svg" width="40" height="40"/>-->     
<!-- DOCKER <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/docker/docker-original.svg" width="40" height="40"/>-->
<!-- KUBERNETS  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/kubernetes/kubernetes-plain.svg" width="40" height="40"/>-->
<!-- JAVA <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/java/java-original.svg" width="40" height="40"/>-->
<!-- JS <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/javascript/javascript-original.svg" width="40" height="40"/>-->
<!-- Node <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/nodejs/nodejs-original.svg" width="40" height="40"/>-->
<!-- EX <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/express/express-original.svg" width="40" height="40"/>-->
<!-- TS <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/typescript/typescript-original.svg" width="40" height="40"/>-->
<!-- React <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/react/react-original.svg" width="40" height="40"/>-->
<!-- Mongo <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/mongodb/mongodb-original.svg" width="40" height="40"/>-->
<!-- PYTHON <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/python/python-original.svg" width="40" height="40"/>-->
<img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" width="40" height="40"/>
<!--  <img src="" width="40" height="40"/>-->
<!--  <img src="" width="40" height="40"/>-->
<!--  <img src="" width="40" height="40"/>-->
<!--  <img src="" width="40" height="40"/>-->
</p>      

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started



### Prerequisites

Is mandatory to have installed C program an the _make_ package. To avoid errors during runtime, use Linux instead Windows. The following examples are for Ubuntu distribution:
* c
  ```sh
  sudo apt install GCC
  ```
### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/iakigarci/KernelSimulator.git
   ```
2. Install C packages
   ```sh
   sudo apt install make
   ```
3. Compile source files
   ```sh
   make
   ```
4. Run the init script. Change the parameters name with the simulation parameters
   ```sh
   ./kernel clockFrequency timerFrequency processGenFrequency
   ```
   For example:
   ```sh
   ./kernel 10 10 10
   ```

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

As we have seen in the previous section, the execution of the program has been simplified with a MAKEFILE file. The MAKEFILE parameters are:

```sh
OBJS	= kernel.o 
SOURCE	= kernel.c 
HEADER	= definitions.h 
OUT	= kernel.out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


kernel.o: kernel.c
	$(CC) $(FLAGS) kernel.c 

queue.o: queue.c
	$(CC) $(FLAGS) queue.c 

thread.o: thread.c
	$(CC) $(FLAGS) thread.c 

mensajes.o: mensajes.c
	$(CC) $(FLAGS) mensajes.c 

clean:
	rm -f $(OBJS) $(OUT)
```


_For more examples, please refer to the [Documentation](Proyecto_SO_IGarcia.pdf) (Spanish)_

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ROADMAP -->
<!--## Roadmap

- [x] Add Changelog
- [x] Add back to top links
- [ ] Add Additional Templates w/ Examples
- [ ] Add "components" document to easily copy & paste sections of the readme
- [ ] Multi-language Support
    - [ ] Chinese
    - [ ] Spanish

See the [open issues](https://github.com/othneildrew/Best-README-Template/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#top">back to top</a>)</p>-->



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Iñaki García : i.garcia.noya@gmail.com

Project Link: [https://github.com/iakigarci/KernelSimulator](https://github.com/iakigarci/KernelSimulator)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTRIBUTORS -->
<!--
## Contributors
* iakigarci [https://github.com/iakigarci/](https://github.com/iakigarci/)

-->

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
