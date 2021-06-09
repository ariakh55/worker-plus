# Worker+
A simple but elegant library to create a local backend for desktop apps.

This library prioritizes the application processing runtime by sending the calculations and processable data from the front-end directly to the local bootstrapper written by c++ using local sockets.

Written with native POSIX functions, I made it possible to use this library on macOS and Windows using cross-compile techniques. (For more information, go to [Usage and Installations](#usage-and-installations))

## How Worker+ works?
Explaining how this library works might help you understand the logic of the background running bootstrapper process running in the **desktop** environment.

Technologies like Electron or Qt WebEngine, allow you to easily create and design User Interfaces for your desktop applications with web design technologies that aren’t optimized to process calculations and heavy data processing. In the meantime, we don’t want to either process or calculate our precious data with front-end technologies and cause memory depletion. Using desktop programming technologies, we can create a backend service for our program to take responsibility for this process and calculations by the fastest runtime. We can split and manage system resources within the range of our application.

Using this concept, we can start designing the User Interface, then connect the front-end to the bootstrapper using a local socket. Easy as ABC!

After we have connected to the bootstrapper; We can send in our flags using string or JSON. Given the flags, we can start processing data and finally send the results back to the front-end.


## Usage and Installations
### Global build
``` bash
g++ -g main.cpp lib/flags.cpp lib/bootstaper.cpp -o main[file extention according to the specified OS] -mwindows
```

### Makefile
Makefile is now added to the library bundle.
Run ```make build``` to have a clean build of the library bundle.
All of the instructions to build for Windows and MacOS are included in the Makefile.

### Linux
Copy ```main.cpp``` and all files in the ```lib``` in the UI working source directory and specify the location of ```package.json``` then Using ```g++``` build the program

### Windows
Do the steps above. but build the ```main.cpp``` along with files in ```lib``` directory using cygwin mingw then build the ```windows_main.cpp``` in order to make a valid executable for Windows system

## Documentaion
This documentaion will contain all techincal documentaion related to the setup, deployment of your application using Worker+

### Port Checker
Starting the program this library check ports from port 49152 to port 65535.
You can change this port range easily by changing the ```for``` statement in line ```88```

```cpp
for(int port = <starting_port>;port <= <starting_port>; ++port)
```

### Settings.json
This file contains the connections infromations between the bootstrapper and the UI application.

```JSON
{
	"port" : 49152,
	"ip" : "0.0.0.0"
}
```

### Flags

You can bind your processings and functions to the main program using the flags.
Flags are mostly recived in String or JSON formats. using the decoder in the program we can decipher the flags and proceed data to calculations.

You can add and manage your flags in the application using the flags header and source file

to fully use your custom flags you can change lines ```134``` - ```140```.

### Front-end connection
After configuring the bootstrapper, we need to configure the UI connection manager.

Using ```connector.js``` in ```src``` directory we can config the flag connections to commuincate between the UI and bootstrapper.

```js
const { resolve } = require('dns');
const net = require('net');
var client = new net.Socket();

let __rootdir = '../../..';

const config = require(`${__rootdir}/settings/connection.json`);

client.connect(config.port, config.ip, () => {
    console.log('connected!');
});

client.on('close', () => {
    console.log('Closed connection!!');
    client.destroy();
});
```

## Notices
In the build process make sure you set the application name correctly (elecron-forge)

```JSON
    "packagerConfig": {
        "asar": true
    },
    "makers": [
    {
        "name": "@electron-forge/maker-squirrel",
        "config": {
        "name": "<app_name>"
        }
    },
```

In main.cpp
```cpp
void signalHandler(int signum){
    cout << "Interrupt signal (" << signum << ") received." << endl;
    system("taskkill /im <app_name>.exe /t /f");
    
    exit(signum);
}

int main(){
		
	int res = system("./<app_name>.exe &");
	if(res == 0)
		cout<< "UI app ready!" << endl;
    
    signal(SIGINT, signalHandler);
    init_bootstrap();

    return 0;
}
```

(Windows) in windows_main.cpp
```cpp
int main(){
	
	system("START /B <app_name>.exe > .elct");
	cout << "UI ready..." << endl;
	system("START /B main.exe > log.txt");
	
	return 0;
} // windows mingw
```