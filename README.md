KeyStatus
=======
After Effects plugin to visualize alpha status when keying
-----------


An After Effects plugin which renders the alpha channel to opaque RGB in an exaggerated way that emphasizes partially transparent pixels. This is useful to check your output when keying, it will help you spot problems such as partially transparent areas on your subject, or partially opaque spots on your background, which might cause problems later. It is similar to the "Status" in KeyLight, except it is a standalone effect so it can be applied anywhere for cases when your keying is multiple steps beyond a simple KeyLight instance. Recommended to put this in an Adjustment Layer, marked as a Guide layer, so that you can easily enable it at any time after adjusting your key.

![Footage](keystatus_footage.png)

Looks like a pretty good key...

![Status](keystatus_status.png)

But there are soft areas which might leak background when composisted, and slightly visible bits in the background. Depending on your composite your results might be fine, but you should at least be aware so you know what to check for. An inner matte to prevent holes in the subject and a junk mask or second differently-hued key might be called for.

# Build and install

## Mac
Download AfterEffectsSDK

Put this project's code into AfterEffectsSDK/Examples/Effect/Keystatus

sudo mkdir /Library/Application\ Support/Adobe/Common/Plug-ins/7.0/MediaCore/KeyStatus

chown the above dir to yourself, or use chgrp chmod or something else to make it writeable by your user account, so that XCode can copy the compiled effect there without permission problems

open XCode project

command-B

## Windows

Untested on Windows, but the VSCode project is up to date and in theory it should work fine if built and installed.

On Windows, the common plug-ins folder can be found (as an explicit path) in the following registry entry: HKLM\SOFTWARE\Adobe\After Effects\[version]\CommonPluginInstallPath


