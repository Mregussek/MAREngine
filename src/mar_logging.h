// ----------------------------------------------
// MAR_ENGINE_COMPILE_SETUP
// ----------------------------------------------

/*
!!! Comment if release mode

Release Mode means here, that all code responsible for logging will not exist.
MAREngine is using macro-definitions for logging such as MAR_CORE_ERROR(x), which is replaced
by actual line of code. Commenting MAR_ENGINE_DEBUG_MODE means all those macro's will be just macro's.

Logging is damaging perfomance, so it will be wise to comment it (Of course if you don't need logging).
*/
#define MAR_ENGINE_DEBUG_MODE

// set logging level
//#define MAR_ENGINE_LOGS_LEVEL_TRACE
//#define MAR_ENGINE_LOGS_LEVEL_INFO
#define MAR_ENGINE_LOGS_LEVEL_WARN
//#define MAR_ENGINE_LOGS_LEVEL_ERROR

/*
!!! Comment if logs should be in logs/log.txt file instead of printed on the terminal

Normally MAREngine was developed with printed on terminal logs, in order to catch something red (such errors).
Also it is needed to store somewhere loggings (let's say in release mode). I saw that, logging to file was not
affecting actual perfomance of engine so much, so I created another option here. Leave uncommented in order to see
every log on terminal.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_TERMINAL_LOGS will not affect anything.
*/
#define MAR_ENGINE_TERMINAL_LOGS


/*
!!! Uncomment to see Editor logs

Uncommenting this macro will create ability to see logs from code, such as GUI editor, managing the whole things,
saving scene to file, load scene from file and others. In summary all things associated to Editor Layer.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_EDITOR_LOGS will not affect anything.
*/
//#define MAR_ENGINE_EDITOR_LOGS


/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


/*
!!! Uncomment to see Graphics logs

Uncommenting this macro will create ability to see logs from code, such as Renderer API, the whole EBO, VBO stuff,
what happens during rendering, calling shaders, mesh creation and others.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_GRAPHICS_LOGS will not affect anything.
*/
#define MAR_ENGINE_GRAPHICS_LOGS


/*
!!! Uncomment to see Entity Component System logs

Uncommenting this macro will create ability to see logs from code, such as Entity Component System stuff. Specifically
entity creation, adding components to it, when components are called. In order to see it proper way, you should add
GRAPHICS_LOGS and EDITOR_LOGS, because ECS are the core of MAREngine. Entities && Components are called wherever needed.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_ECS_LOGS will not affect anything.
*/
#define MAR_ENGINE_ECS_LOGS

/*
!!! Uncomment to see Platform Logs
*/
//#define MAR_ENGINE_SCRIPTING_LOGS


/*
!!! Uncomment to see Layer Logs

MAREngine is updating the whole frame with LayerStack, where each layer calls its overrided update() method. Sometimes
it is usefull to see, if at which point each layer is.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_LAYER_LOGS will not affect anything.
*/
//#define MAR_ENGINE_LAYER_LOGS

/*
!!! Uncomment to see Window Logs

Everything is displayed on window, so logging from window itself might be helpful. Uncommenting this macro
will let you catch, when input is enabled, which key is pressed or operations on window like cleaning, swapping buffers.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_WINDOW_LOGS will not affect anything.
*/
//#define MAR_ENGINE_WINDOW_LOGS

/*
!!! Uncomment to see Platform Logs
*/
#define MAR_ENGINE_PLATFORM_LOGS