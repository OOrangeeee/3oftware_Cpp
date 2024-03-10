
/*
    设置一个cur_path存储当前剩余的没走完的路
    （不同状态不另设变量表示，通过考察现有的各种变量来区分）
*/

//struct Robo_situation
{
    int cur_path;//the path remaining for the robot to move
    bool at_berth();
    /*
        return robot->pos == robot->berth_pos;
    */
}

/*
    下面的步骤仅供最开始机器人前往各自的港口时执行：
    1. 判断各机器人到各港口是否有路（只需判断是否有路即可，无需求最短路，不知道有没有比dfs剪枝更好的方法）
    2. 寻找一种匹配方案，尽量使每个机器人都有一个不同的可达港口
    3. 根据上述找到的方案，规划最短路（10条单源最短路），存入cur_path中
*/

void go_to_berth_initially()//runs only once when initialization
{
    for(each robot and each berth)
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

/*
    每帧生成机器人指令时，按照如下方案：
    1. 首先，判断cur_path是否为空（是否走完了之前安排好的道路），如果未走完，按照cur_path下达移动一步的指令即可，结束；否则，进行2
    //1不成立说明已经走完了之前安排好的路，现在，机器人要么在港口，要么在货物点
    2. 判断当前机器人是否在港口且手中有货物，若是，则安排放下货物的指令，同时分配一个合法的新货物给它（规划到该货物的最短路，判断是否能在消失前取到）
        ，并将该最短路存入cur_path中，结束；否则，执行3
    //若机器人在港口，且之前已经放下的货物，则之前在放下货物时已经规划好了路线存入cur_path中，此时机器人状态属于1成立的情况
    //1,2都不成立说明机器人在货物点
    3. 判断当前机器人手中是否有货物（其实不用判断了，只剩这一种情况），若没有，则下达拿起货物的指令，同时将从港口来时的反向路存入cur_path中，结束
    //若机器人在货物点，且之前已经拿起货物，则之前拿起货物时已经规划好了路线存入cur_path中，此时机器人状态属于1成立的情况
*/

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
