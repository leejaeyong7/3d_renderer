/*============================================================================
 * @author: Jae Yong Lee
 * @file: simFloor.h
 * @description:
 *     Declarartion file for floor environment
 *
 *============================================================================*/
class SimFloor : SimEnvironment
{
    public:
        /**
         * Default constructor; floor only needs height
         */
        SimFloor(std::string _name, double y, std::string _meshPath);


    private:
};
