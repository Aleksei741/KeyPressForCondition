#pragma once
#include <Windows.h>



#define TAB_PAGES	6
#define TAB_COMPONENTS	100
#define TAB_TABLECOMPONENTS	10

#define START_ID_BUTTON	2000

#define TAB_PAGE_TIMER	0
#define TAB_PAGE_PIXELEVENTS	1

#define FIELDS_TIMER_PAGE 5 
#define FIELD_ACTIVE_TIMER_PAGE(x)				(0 + FIELDS_TIMER_PAGE * x)
#define FIELD_BUTTON_TIMER_PAGE(x)				(1 + FIELDS_TIMER_PAGE * x) 
#define FIELD_PERIOD_TIMER_PAGE(x)				(2 + FIELDS_TIMER_PAGE * x) 
#define FIELD_NUMREPEAT_TIMER_PAGE(x)			(3 + FIELDS_TIMER_PAGE * x)
#define FIELD_DELAY_TIMER_PAGE(x)				(4 + FIELDS_TIMER_PAGE * x)

#define ID_ACTIVE_TIMER_PAGE					(2000)
#define ID_BUTTON_TIMER_PAGE					(2001)
#define ID_PERIOD_TIMER_PAGE(x)					(2020 + x)
#define ID_NUMREPEAT_TIMER_PAGE(x)				(2040 + x)
#define ID_DELAY_TIMER_PAGE(x)					(2060 + x)

#define FIELDS_PIXELEVENTS_PAGE 9 
#define FIELD_ACTIVE_PIXELEVENTS_PAGE(x)		(0 + FIELDS_PIXELEVENTS_PAGE * x)
#define FIELD_BUTTON_PIXELEVENTS_PAGE(x)		(1 + FIELDS_PIXELEVENTS_PAGE * x) 
#define FIELD_CONDITION_PIXELEVENTS_PAGE(x)		(2 + FIELDS_PIXELEVENTS_PAGE * x)
#define FIELD_SETCOLOR_PIXELEVENTS_PAGE(x)		(3 + FIELDS_PIXELEVENTS_PAGE * x)
#define FIELD_GETCOLOR_PIXELEVENTS_PAGE(x)		(4 + FIELDS_PIXELEVENTS_PAGE * x)
#define FIELD_BUTTONSETPIXEL_PIXELEVENTS_PAGE(x)	(5 + FIELDS_PIXELEVENTS_PAGE * x)
#define FIELD_POSITIONPIXEL_PIXELEVENTS_PAGE(x)	(6 + FIELDS_PIXELEVENTS_PAGE * x)
#define FIELD_PERIOD_PIXELEVENTS_PAGE(x)		(7 + FIELDS_PIXELEVENTS_PAGE * x)
#define FIELD_DELAY_PIXELEVENTS_PAGE(x)			(8 + FIELDS_PIXELEVENTS_PAGE * x)

#define ID_ACTIVE_PIXELEVENTS_PAGE				(2100)
#define ID_BUTTON_PIXELEVENTS_PAGE				(2101)
#define ID_CONDITION_PIXELEVENTS_PAGE			(2102)
#define ID_PERIOD_PIXELEVENTS_PAGE(x)			(2120 + x)
#define ID_BUTTONSETPIXEL_PIXELEVENTS_PAGE(x)	(2140 + x)
#define ID_DELAY_PIXELEVENTS_PAGE(x)			(2160 + x)

enum {
	OnMenuActionSave,
	OnMenuActionOpen,

	StartButtonClik,
	StopButtonClik,

	SearchWindowHandlesButtonClik,

	ID_COMPONENTSUSER,

	ID_LISTBOX_SearchWindowHandles,
	ClickButtonSearchWindowHandles,

	CheckBoxUSB,
	CheckBoxMarkPixel,	

	OnExitSoftware,
};

