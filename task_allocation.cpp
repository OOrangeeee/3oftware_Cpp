
//struct Robo_situation
{
    int cur_path;//the path remaining for the robot to move
    bool at_berth();
    /*
        return robot->pos == robot->berth_pos;
    */
}


void go_to_berth_initially()//runs only once when initialization
{
    for(each robots)
    {
        find_path(robo_cur_loc, target_berth);//it has to be reachable, which doesn't need to be a shortest one
    }
    match_robots_to_berths();//ensure that the berth allocated to the robot has to be reachable
    /*
        match each robot to berth by the algorithm you have already written
        and for each, remember to update the info as follows:
        {
            robot->goal_pos = (robot->berth_pos = target_berth_pos)
            robot->berth_id = target_berth_id
        }
    */
    creat_path(robot->pos, robot->goal_pos);
    /*
        target_path = find_shortest_path(robot->pos, robot->goal_pos)
        robot->cur_path = from_path_to_move(target_path) //transform the path into the move order(0/1/2/3)
    */
}

void robot_action_generate()
{
    if(robot->cur_path is not empty)
    {
        order_out(move, robot->id, robot->cur_path[0]);
        cur_path_pop(robot->cur_path);//delete the first move of cur_path
        return;
    }
    else //cur_path is empty, which means the moves of cur_path has all been finished already
    {
        if(at_berth() && robot->is_has)//the robot is at its berth and has its goods in his hand
        {
            order_out(pull, robot_id);
            allocate_new_good(robot); //now it has a new path to move
            /*
                pop one good from the waiting-list of the robot's berth
                robot->goal_pos = target_good_pos
                creat_path(robot->pos, robot->goal_pos)
            */
            return;
            //else, it is at its berth and has gained a path planned when he pulled the goods at last frame
        }
        else //it is at the position of its target good
        {
            if(!robot->it_has) //the robot haven't taken the good yet
            {
                order_out(get, robot->id);
                robot->goal_pos = robot->berth_pos
                creat_path(robot->pos, robot->goal_pos)
                return;
            }
            //else, it got its good just now and has gained a path planned when he got the goods at last frame
        }
    }
}