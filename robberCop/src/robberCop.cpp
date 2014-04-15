#include "sgct.h"
#include "Scene.h"
#include "robberCop.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

void RobberCop::process(int id, int action, bool value) {
    if(id > scene->player_vec.lenth()) {
        scene->addPlayer();
        scene->player_vec[id].setMoveDirection(action, value);
    }
    else{
        scene->player_vec[id].setMoveDirection(action, value);
    }
}