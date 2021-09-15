# LOAD MONSTER
[![](https://github.com/sudara/load_monster_plugin/workflows/CMake/badge.svg)](https://github.com/sudara/load_monster_plugin/actions)

![](https://user-images.githubusercontent.com/472/133512219-50bf5f4a-2659-4c5a-9ddb-cc6754284f8b.mp4)

## What?

It makes no noise! 

It just eats ur (audio) brains! (CPU).

## Why?

While chewing on another plugin project, I noticed Logic's CPU metering was behaving strangely. It seemed to be claiming higher CPU usage than what I knew objectively my plugin was using and behaved strangely with large spikes > 50%.


![](https://user-images.githubusercontent.com/472/133512250-fcee99be-d8d2-46b7-8435-3665f8dd9b43.mp4)


After measuring actual usage a few other different ways ([I <3 perfetto!](https://perfetto.dev)) I came to the conclusion that creating a measurment plugin was the only way to figure out what was going on here...

## Building

You'll need CMake.

Run:

`git clone --recurse-submodules https://github.com/sudara/load_monster_plugin.git`

Or if you've already cloned the repo you can, grab submodules:

```
git submodule update --init
```

Then run your CMake stuff. For example, for a MacOS Xcode project:

```
cmake -B Builds -G Xcode
```

Better to just use Clion though, it'll handle all your CMakin'.

## Acknowledgements

* The plugin template is [Pamplejuce](http://github.com/sudara/pamplejuce).
* The Monster is from https://openmoji.org/ and licensed CC BY-SA 4.0.
