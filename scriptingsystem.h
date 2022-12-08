#ifndef SCRIPTINGSYSTEM_H
#define SCRIPTINGSYSTEM_H

/// Used to run javascript files and change the speed of the player.
class ScriptingSystem
{
public:
    ScriptingSystem();
    void runScript();
    float carSpeed{0.2f};
};

#endif // SCRIPTINGSYSTEM_H
