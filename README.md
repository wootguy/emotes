# emotes

Commands for testing player model animations. Basically, it turns the game into VR chat.

```

Type ".e list" to list named emotes.
Type ".e off" to stop your emote.
Type ".e <name> [speed]" to play a named emote.
Type ".e <sequence> [mode] [speed] [start_frame] [end_frame]" for more control.

<> = required. [] = optional.

<sequence> = 0-255. Most models have about 190 sequences.
[mode] = ONCE, FREEZE, LOOP, or ILOOP. Doesn't have to be all caps.
[speed] = Any number, even negative. The default speed is 1.
[start_frame/end_frame] = 0-255. This is like a percentage. Frame count in the model doesn't matter.
```

# Installation
1. Download the latest [release](https://github.com/wootguy/emotes/releases) and extract to svencoop_addon. The updated `player.mdl` is required for animations to work beyond `190`. Only the server needs this model installed.
1. Add this to `default_plugins.txt`
```
  "plugin"
  {
    "name" "emotes"
    "script" "emotes"
  }
```
