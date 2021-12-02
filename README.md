# ESP Endpoint library for [PlatformIO](https://github.com/platformio)

Create a simple server on your ESP8266 module that manages custom named integers accessible through GET requests and executes callbacks when they are changed.

## Usage

Add this repository's URL to your `.platformio.ini`'s `lib_deps` and import the library in your source file as follows.

```cpp
#import <esp_endpoint.hpp>
```

To get started, create an Array of type `ESPEndpoint::Variable` where each element is a struct consisting of the variable's name, a label, value (of type `int`) at initialization and pointer to the respective callback function.

```cpp
int my_callback(int my_var) {
    return my_var;
}

...

ESPEndpoint::Variable my_vars[] = {
    { "my_var", "my_label", 0, my_callback },
    ...
};
```

Next, create *one* instance of `ESPEndpoint` by providing your Wi-Fi's SSID and password, giving your server a name and passing your array and count of variables.

```cpp
ESPEndpoint my_endpoint { WIFI_SSID, WIFI_PASSWORD, "my endpoint" my_vars, 1 };
```

At initialization, the endpoint will print its assigned IP address through the `Serial` connection if it is enabled. This is one way of finding out your ESP's IP address. Additionally, your ESP will frequently send gratuitous ARP requests to ensure presence in ARP cache of various devices, meaning you can always find it by running a simple `$ arp -a`.

The only thing left to do now is call `ESPEndpoint::iterate()` in your run loop.

```cpp
my_endpoint.iterate();
```

## Server requests

Your ESP will serve at its IP address and standard port and return a JSON dictionary including the following entries:

- `id` with the server's given name and `vars` with an array of variable names, labels and current values if the requested path includes the string `id`
- one or multiple given variables' names along with their current values if their name is included in the requested path

If the character `=` and digits follow a given variable's name, the digits will (greedily) be connected into an integer and the given variable's callback function will be called with this integer. The variable's value will be updated to become the return value of the callback. The return dictionary will then include this updated value. Multiple variable assignments can be separated by any URL-valid character that is not a digit.

In the provided example the request `<IP>/my_var` will return a dictionary with the key `my_var` and the variable's current value **without** executing the callback. The request `<IP>/my_var=10` will execute the callback, assign its return value `10` to `my_var` and return the same dictionary, now with the value of `10`. The `<IP>/id` request will then respond with the following JSON.

```json
{
    "id": "my endpoint",
    "vars": [
        { "name": "my_var", "label": "my_label", "val": 10 }
    ]
}
```

Assigning values multiple variables and / or getting their current value can be chained together using separation characters such as `&`, e.g. `<IP>/my_var1=10&my_var2=20` will set both variables and respond with their respective values.

