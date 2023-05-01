# Rebind2
Rebind2 works by remapping key inputs on X11 applications by LD_PRELOAD the 
XNextEvent function. 
After intercepting XEvents, it filters based on a given configuration file
what events should be passed as normal and what should be changed
## Usage
To overwrite keys a file must first be made with
```
./create_mapping /home/ubuntu/input /home/ubuntu/output
```
Note that input files must be in the format of  
[keycode] [keystate] [keycode] [keystate]  
Where the keycode and state are defined by the X11 definitions. Lines that start with #
are ignored. More examples below. To overwrite the bindings, the environment variables
LD_PRELOAD and MAPPINGS must be set to the absoulte path of the shared object and the
output of ./create_mapping. The following would rebind firefox
```
LD_PRELOAD=/home/ubuntu/rebind_preload.so MAPPINGS=/home/ubuntu/output firefox
```
## File Format Examples
To rebind "Ctrl+[" to "escape" and "a" to "A" in an application,
the following configuration file would need to be passed though create_mapping
```
# lines that start with hashtag will be ignored
# remaps ctrl+[ to esc because [ has keycode 34 and state of 4 (control)
# while escape has keycode of 9 and state of 0 (no modifier)
34 4 9 0

# note that more bindings can be added below
# remaps h to H
43 0 43 1
```







