<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/hehe1005566889/FMusicCLI">
    <img src="readme/80.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">FMusicCore</h3>

  <p align="center">
    A Music Library For Common Online Music Platform
    <br />
    <a href="https://github.com/hehe1005566889/FMusicCLI"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/hehe1005566889/FMusicCLI">View Demo</a>
    ·
    <a href="https://github.com/hehe1005566889/FMusicCLI/issues">Report Bug</a>
    ·
    <a href="https://github.com/hehe1005566889/FMusicCLI/issues">Request Feature</a>
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
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This Project Contains Common Online Music Platforms APIs. You Can Use Any Lanague To Call The API (Contains Bindings) To Fetch Music Info , Play Music Uri , Searching Music , User Options And So On

We Support Java And C# Binding And C++ APIs At Now.This Project Use Async Request And Fast Music Editor To Deal With Music Cover, And Mate Infos.

FMusicCore(CLI) Is MultiPlatform And Fast. Also It Can Modify Very Simple, Just Using Stand C++ And ReWrite The MusicAPI Interface Class, To Add A New Music Platform Support For It.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

* VisualStudio Code
* CMake

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

How To Use It? Download Binary File From The Release Page. This Binary Is CLI Version, Which Only Contains Console UI(Using FTXUI). It Can Runs On Your Console For Basic Functions. Such As Download Music And Serach Music ...

How To Develop With It? Read The 'Prerequisites' And 'Compilation' To Setup Your Project With 'FMusicCore'

### Prerequisites

We Need These Libraries For Project Dependencis
* HTTPRequest (Must)
* json (Must, Gen With JsonCpp Library)
* taglib (Must)

* FTXUI (Optional, CLI Version Must)

* Java (Optional, Java Binding Mode Must)
* .Net7 (Optional, C# Binding Mode Must)

Also If You Want To Compile The Core Code, You Need A Environment Support CMake Build System
And C/C++ Compiler

### Compilation

1. Check Your Environment, Sure That A Things Install
2. Clone This Repo To Your Computer, We Suggest Clone To A ASCII Named Folder
   ```sh
   git clone https://github.com/hehe1005566889/FMusicCLI
   ```
3. Sync All Git SubModules. And Init All SubModules.
   ```sh
   git submodule sync
   ```
4. Open Your Project In Your IDE Or CMake GUI Tool To Gen Project And Compile
5. If You Need To Using FMusicCore In Outher Language. Run Platform Scripts In 'Scripts' Folder To Compile Your Language Binding

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

For Examples And Useage You Can Go To Our Repo Document

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [ ] TODO
- [ ] TODO
- [ ] TODO
    - [ ] TODO

See the [open issues](https://github.com/hehe1005566889/FMusicCLI/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

TODO

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the BSD-2 License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo_name.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo_name/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo_name.svg?style=for-the-badge
[forks-url]: https://github.com/github_username/repo_name/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo_name.svg?style=for-the-badge
[stars-url]: https://github.com/github_username/repo_name/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo_name.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo_name/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo_name.svg?style=for-the-badge
[license-url]: https://github.com/github_username/repo_name/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
