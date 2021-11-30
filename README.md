# ESP Endpoint library for [PlatformIO](https://github.com/platformio)

Create a simple server on your ESP8266 module that manages custom named integers accessible through GET requests and executes callbacks when they are changed.

## Usage

Add this repository's URL to your `.platformio.ini`'s `lib_deps` and import the library in your source file as follows.

```cpp
#import <esp_endpoint.hpp>
```

To get started, create an Array of type `ESPEndpoint::Variable` where each element is a struct consisting of the variable's name, value (of type `int`) at initialization and pointer to the respective callback function.

```cpp
int my_callback(int my_var) {
    return my_var;
}

...

ESPEndpoint::Variable my_vars[] = {
    { "my_var", 0, my_callback },
    ...
};
```

Next, create *one* instance of `ESPEndpoint` by providing your Wi-Fi's SSID and password, giving your server a name and passing your array and count of variables.

```cpp
ESPEndpoint my_endpoint { WIFI_SSID, WIFI_PASSWORD, "my endpoint" my_vars, 1 };
```

At initialization, the endpoint will print its assigned IP address through the `Serial` connection if it is enabled. This is one way of finding out your ESP's IP address.

The only thing left to do now is call `ESPEndpoint::iterate()` in your run loop.

```cpp
my_endpoint.iterate();
```

## Server requests

Your ESP will serve at its IP address and standard port and return a JSON dictionary including the following entries:

- `name` with the server's given name if the requested path includes the string `info`
- one or multiple given variables' names along with their current values if their name is included in the requested path

If the character `=` and digits follow a given variable's name, the digits will (greedily) be connected into an integer and the given variable's callback function will be called with this integer. The variable's value will be updated to become the return value of the callback. The return dictionary will then include this updated value. Multiple variable assignments can be separated by any URL-valid character that is not a digit.

In the provided example the request `<IP>/my_var` will return a dictionary with the key `my_var` and the variable's current value **without** executing the callback. The request `<IP>/my_var=10;info` will execute the callback, assign its return value `10` to `my_var` and return the following JSON.

```json
{
    "name": "my endpoint",
    "my_var": 10,
}
```

