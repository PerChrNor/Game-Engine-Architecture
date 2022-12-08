#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "resourcemanager.h"

/// This Class handles all collision response.
///
/// Here all collision detection has already happened this class is in charge of all collision resonses that happenes in the scene.
/// If any new interaction is wanted then that function is placed here.
/// this class whole was created by student nr:8002
class Collisions
{
public:
    Collisions();

    ResourceManager* mCurrentManager{nullptr};

    ///This function is to setup any variables that cant be done in the constuctor.
    ///
    ///Currently saves the player and the players position.
    void initCollisions();

    ///This function is running all collision responses.
    ///
    ///The function is called outside of the class to run all collisions every frame, this is is to ensure that a response can happen on any frame.
    void checkAllCollisions();

    ///This function searches through the Entities to find the entitiy tagged with the name "Player".
    ///
    /// this function is only meant to be run once, this is becaus it is used to save the variable,
    /// when the variable has been saved is is faster to use the saved variable rather than doning another linear search.
    /// @returns the first object tagged player
    /// @see getTracks()
    Entity* getPlayer();

    ///This function searchers for and saves all object with the tag "Track".
    ///
    ///The function is similar to getPlayer() where it does a linear search for any "Track" object, this search is however done on the player to see what track the player is colliding with,
    /// since the player moves the search is done every frame since the objects is collides with changes.
    /// @returns a vector of Entities tagged "Track".
    ///@see getPlayer().
    std::vector <Entity*> getTracks();

    ///This function checks the players height using barysentric coordinates.
    ///
    ///this function searches through each "Track" to find out which triangle the player is standing on, if the triangle is not found it gives a set dummy value.
    /// @returns the correct height is it is found, if not it returns -10000.
    float checkPlayerHeight();

    ///This function sets the players height.
    ///
    ///the function sets the heiht of the player returned from the checkPlayerHeight() function, if the value returned is -10000, the player height is not set.
    ///@param height This is the height returned from checkPlayerHeight().
    ///@see checkPlayerHeight().
    void setPlayerheight(float height);

    ///The playerheight is what the player characters height will be set to in the setPlayerheight() function.
    float playerHeight = 0;

private:
    ///This variable pointer is where the player entity is saved for later use
    ///
    ///Any time a function asks information about the player this variable is used.
    Entity* mPlayerEntity{0};

    ///This is a vector of the Tracks the player is currently collidiong with.
    ///
    ///It is a vector to make sure there is no bugs where the player would be on multiple track parts at the same time.
    std::vector <Entity*> mTrackEntities;

    ///The current possision of the player.
    QVector3D mPlayerPosition;

    ///This bool is to make sure the while-loop stops when it has gone through all the objects,
    ///if this was not used there would be a segmentation fault due to the while-loop trying to check tracks that does not exist.
    bool breaking = false;
};

#endif // COLLISIONS_H
