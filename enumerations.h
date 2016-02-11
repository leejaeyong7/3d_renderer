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
    GUI_ID_QUIT_BUTTON = 0x10000,

    // used for add prompt
    GUI_ID_ENTITY_PROMPT_WINDOW,
    GUI_ID_CLOSE_BUTTON,

    GUI_ID_ENTITY_PROMPT_POS_X,
    GUI_ID_ENTITY_PROMPT_POS_Y,
    GUI_ID_ENTITY_PROMPT_POS_Z,
    GUI_ID_ENTITY_PROMPT_ROT_A,
    GUI_ID_ENTITY_PROMPT_ROT_B,
    GUI_ID_ENTITY_PROMPT_ROT_C,
    GUI_ID_ENTITY_PROMPT_NAME,

    GUI_ID_ADD_ENTITY_ROBOT,
    GUI_ID_ADD_ENTITY_SENSOR,
    GUI_ID_ADD_ENTITY_ENVIRONMENT,

    GUI_ID_EDIT_ENTITY_ROBOT,
    GUI_ID_EDIT_ENTITY_SENSOR,
    GUI_ID_EDIT_ENTITY_ENVIRONMENT,

    GUI_ID_SENSOR_CAM_FOV_X,
    GUI_ID_SENSOR_CAM_FOV_Y,
    GUI_ID_SENSOR_CAM_FOCAL_LENGTH,



    GUI_ID_ENTITY_PROMPT_COMBO,
    GUI_ID_ENTITY_PROMPT_COMBO_TEXT,
    GUI_ID_ENTITY_PROMPT_CONFIRM_BUTTON,
    GUI_ID_ENTITY_PROMPT_REMOVE_BUTTON,
    GUI_ID_ENTITY_PROMPT_ADVANCED_BOX
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
