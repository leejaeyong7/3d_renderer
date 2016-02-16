/*============================================================================
 * @author: Jae Yong Lee
 * @file: enumerations.h
 * @version:  
 * @summary:
 *      Includes all enumerations used commonly throughout simulator
 *
 *============================================================================*/
//----------------------------------------------------------------------------//
//                                Include Guards
//----------------------------------------------------------------------------//
#ifndef _SIM_ENUM_H_
#define _SIM_ENUM_H_
#include <string>

//----------------------------------------------------------------------------//
//                                Enumerations
//----------------------------------------------------------------------------//
typedef enum optionType{
    STRING,
    DOUBLE,
    INTEGER,
} OptionType;

typedef enum entityType{
    ENTITY_TYPE_ROBOT,
    ENTITY_TYPE_SENSOR,
    ENTITY_TYPE_ENVIRONMENT
} EntityType;

enum
{
    QUIT_BUTTON = 0x10000,

    // used for add prompt
    PROMPT_ADD_WINDOW,
    PROMPT_EDIT_WINDOW,
    PROMPT_COMBO,
    PROMPT_NAME,
    PROMPT_ADVANCED_BOX,

    PROMPT_POS_X,
    PROMPT_POS_Y,
    PROMPT_POS_Z,
    PROMPT_ROT_A,
    PROMPT_ROT_B,
    PROMPT_ROT_C,
    PROMPT_ADV_INPUT,

    CLOSE_BUTTON,
    CONFIRM_BUTTON,
    REMOVE_BUTTON,

    ADD_ROBOT,
    ADD_SENSOR,
    ADD_ENVIRONMENT,

    EDIT_ROBOT,
    EDIT_SENSOR,
    EDIT_ENVIRONMENT
};

enum
{
    SUB_ENTITY_ROBOT_QUAD,
    SUB_ENTITY_ROBOT_GROUND,
    SUB_ENTITY_SENSOR_CAM,
    SUB_ENTITY_ENVIRONMENT_APRIL,
    SUB_ENTITY_ENVIRONMENT_CUBE,
    SUB_ENTITY_ENVIRONMENT_SPHERE
};
enum{
    ADD_ENTITY_PROMPT = 0x5000,
    EDIT_ENTITY_PROMPT
};

#endif
