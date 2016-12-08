
/*
   LVDLG.H - LEAD Vector Dialog module header file
   Copyright (c) 1991-2000 LEAD Technologies, Inc.
   All Rights Reserved.
*/

#ifndef _LVDLG_H_
#define _LVDLG_H_

#include "ltkrn.h"
#include "lvkrn.h"

#define _HEADER_ENTRY_
#include "ltpck.h"

/* definitions */
#define VECTOR_DLG_SHOW_PREVIEW (0x40000000)
#define VECTOR_DLG_AUTO_PROCESS (0x80000000)

#define VECTOR_DLG_SELECTED_ONLY (0x00000001)
#define VECTOR_DLG_NO_RETURN_DUPLICATE (0x00000001)

/* Dialog String Indices */
enum
{
    /* Generic control strings */
    VECTOR_DLGSTR_BUTTON_OK,
    VECTOR_DLGSTR_BUTTON_CANCEL,
    VECTOR_DLGSTR_BUTTON_HELP,
    VECTOR_DLGSTR_CHECK_PREVIEW,

    /* Scale, Rotate, Translate, and Camera Dialog */
    VECTOR_DLGSTR_TRANSFORM_SCALE_CAPTION,
    VECTOR_DLGSTR_TRANSFORM_ROTATE_CAPTION,
    VECTOR_DLGSTR_TRANSFORM_TRANSLATE_CAPTION,
    VECTOR_DLGSTR_TRANSFORM_CAMERA_CAPTION,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_XUP,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_XDOWN,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_YUP,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_YDOWN,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_ZUP,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_ZDOWN,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_XUPCAMERA,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_XDOWNCAMERA,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_YUPCAMERA,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_YDOWNCAMERA,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_ZUPCAMERA,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_ZDOWNCAMERA,
    VECTOR_DLGSTR_TRANSFORM_CHECK_ALL,
    VECTOR_DLGSTR_TRANSFORM_CHECK_PERSPECTIVE,
    VECTOR_DLGSTR_TRANSFORM_BUTTON_RESET,

    /* Render dialog */
    VECTOR_DLGSTR_RENDER_CAPTION,
    VECTOR_DLGSTR_RENDER_FRAME_LIGHTS,
    VECTOR_DLGSTR_RENDER_CHECK_USELIGHTS,
    VECTOR_DLGSTR_RENDER_BUTTON_AMBIENT,
    VECTOR_DLGSTR_RENDER_FRAME_POLYGONMODE,
    VECTOR_DLGSTR_RENDER_RADIO_POINT,
    VECTOR_DLGSTR_RENDER_RADIO_LINE,
    VECTOR_DLGSTR_RENDER_RADIO_FILL,
    VECTOR_DLGSTR_RENDER_RADIO_FILL_RASTER_ALWAYS,

    /* View mode dialog */
    VECTOR_DLGSTR_VIEWMODE_CAPTION,
    VECTOR_DLGSTR_VIEWMODE_FRAME_VIEWMODE,
    VECTOR_DLGSTR_VIEWMODE_RADIO_FIT,
    VECTOR_DLGSTR_VIEWMODE_RADIO_SNAP,

    VECTOR_DLGSTR_HITTEST_CAPTION,
    VECTOR_DLGSTR_HITTEST_FRAME_DISTANCE,
    VECTOR_DLGSTR_HITTEST_FRAME_OPTIONS,
    VECTOR_DLGSTR_HITTEST_CHECK_CLOSEDFIGURES,
    VECTOR_DLGSTR_HITTEST_CHECK_IGNORESELECTED,

    /* All layers dialog */
    VECTOR_DLGSTR_ALLLAYERS_CAPTION,
    VECTOR_DLGSTR_ALLLAYERS_BUTTON_NEW,
    VECTOR_DLGSTR_ALLLAYERS_BUTTON_EDIT,
    VECTOR_DLGSTR_ALLLAYERS_BUTTON_DELETE,
    VECTOR_DLGSTR_ALLLAYERS_YES,
    VECTOR_DLGSTR_ALLLAYERS_NO,
    VECTOR_DLGSTR_ALLLAYERS_NAME,
    VECTOR_DLGSTR_ALLLAYERS_ACTIVE,
    VECTOR_DLGSTR_ALLLAYERS_VISIBLE,
    VECTOR_DLGSTR_ALLLAYERS_LOCKED,

    /* Layer dialog */
    VECTOR_DLGSTR_LAYER_CAPTION,
    VECTOR_DLGSTR_LAYER_FRAME_NAME,
    VECTOR_DLGSTR_LAYER_CHECK_ACTIVE,
    VECTOR_DLGSTR_LAYER_CHECK_VISIBLE,
    VECTOR_DLGSTR_LAYER_CHECK_LOCKED,

    /* All groups dialog */
    VECTOR_DLGSTR_ALLGROUPS_CAPTION,
    VECTOR_DLGSTR_ALLGROUPS_BUTTON_NEW,
    VECTOR_DLGSTR_ALLGROUPS_BUTTON_EDIT,
    VECTOR_DLGSTR_ALLGROUPS_BUTTON_DELETE,
    VECTOR_DLGSTR_ALLGROUPS_YES,
    VECTOR_DLGSTR_ALLGROUPS_NO,
    VECTOR_DLGSTR_ALLGROUPS_NAME,

    /* Group dialog */
    VECTOR_DLGSTR_GROUP_CAPTION,
    VECTOR_DLGSTR_GROUP_FRAME_NAME,

    /* General dialog */
    VECTOR_DLGSTR_GENERAL_CAPTION,
    VECTOR_DLGSTR_GENERAL_FRAME_SELECTION,
    VECTOR_DLGSTR_GENERAL_CHECK_SELECTION,
    VECTOR_DLGSTR_GENERAL_FRAME_RASTEROPERATION,

    /* ROP2 */
    VECTOR_DLGSTR_GENERAL_ROP2_BLACKNESS,
    VECTOR_DLGSTR_GENERAL_ROP2_NOTMERGEPEN,
    VECTOR_DLGSTR_GENERAL_ROP2_MASKNOTPEN,
    VECTOR_DLGSTR_GENERAL_ROP2_NOTCOPYPEN,
    VECTOR_DLGSTR_GENERAL_ROP2_MASKPENNOT,
    VECTOR_DLGSTR_GENERAL_ROP2_INVERT,
    VECTOR_DLGSTR_GENERAL_ROP2_XORPEN,
    VECTOR_DLGSTR_GENERAL_ROP2_NOTMASK,
    VECTOR_DLGSTR_GENERAL_ROP2_MASKPEN,
    VECTOR_DLGSTR_GENERAL_ROP2_NOTXORPEN,
    VECTOR_DLGSTR_GENERAL_ROP2_NOP,
    VECTOR_DLGSTR_GENERAL_ROP2_MERGENOTPEN,
    VECTOR_DLGSTR_GENERAL_ROP2_COPYPEN,
    VECTOR_DLGSTR_GENERAL_ROP2_MERGEPENNOT,
    VECTOR_DLGSTR_GENERAL_ROP2_MERGEPEN,
    VECTOR_DLGSTR_GENERAL_ROP2_WHITENESS,

    /* Pen dialog */
    VECTOR_DLGSTR_PEN_CAPTION,
    VECTOR_DLGSTR_PEN_FRAME_STYLE,
    VECTOR_DLGSTR_PEN_FRAME_WIDTH,
    VECTOR_DLGSTR_PEN_FRAME_COLOR,

    /* Pen styles */
    VECTOR_DLGSTR_PEN_STYLE_SOLID,
    VECTOR_DLGSTR_PEN_STYLE_DASH,
    VECTOR_DLGSTR_PEN_STYLE_DOT,
    VECTOR_DLGSTR_PEN_STYLE_DASHDOT,
    VECTOR_DLGSTR_PEN_STYLE_DASHDOTDOT,
    VECTOR_DLGSTR_PEN_STYLE_TRANSPARENT,
    VECTOR_DLGSTR_PEN_STYLE_INSIDEFRAME,

    /* Brush dialog */
    VECTOR_DLGSTR_BRUSH_CAPTION,
    VECTOR_DLGSTR_BRUSH_FRAME_STYLE,
    VECTOR_DLGSTR_BRUSH_FRAME_HATCH,
    VECTOR_DLGSTR_BRUSH_FRAME_COLOR,

    /* Brush styles */
    VECTOR_DLGSTR_BRUSH_STYLE_SOLID,
    VECTOR_DLGSTR_BRUSH_STYLE_TRANSPARENT,
    VECTOR_DLGSTR_BRUSH_STYLE_HATCHED,

    /* Brush hatched */
    VECTOR_DLGSTR_BRUSH_HATCHED_HORIZONTAL,
    VECTOR_DLGSTR_BRUSH_HATCHED_VERTICAL,
    VECTOR_DLGSTR_BRUSH_HATCHED_FORWARDDIAGONAL,
    VECTOR_DLGSTR_BRUSH_HATCHED_BACKWARDDIAGONAL,
    VECTOR_DLGSTR_BRUSH_HATCHED_CROSS,
    VECTOR_DLGSTR_BRUSH_HATCHED_DIAGONALCROSS,

    /* Vertex dialog */
    VECTOR_DLGSTR_VERTEX_CAPTION,
    VECTOR_DLGSTR_VERTEX_FRAME_LOCATION,

    /* Line dialog */
    VECTOR_DLGSTR_LINE_CAPTION,
    VECTOR_DLGSTR_LINE_FRAME_STARTPOINT,
    VECTOR_DLGSTR_LINE_FRAME_ENDPOINT,

    /* Rectangle dialog */
    VECTOR_DLGSTR_RECTANGLE_CAPTION,
    VECTOR_DLGSTR_RECTANGLE_FRAME_STARTPOINT,
    VECTOR_DLGSTR_RECTANGLE_FRAME_ENDPOINT,

    /* Poly Line dialog */
    VECTOR_DLGSTR_POLYLINE_CAPTION,
    VECTOR_DLGSTR_POLYLINE_FRAME_POINTS,
    VECTOR_DLGSTR_POLYLINE_BUTTON_NEW,
    VECTOR_DLGSTR_POLYLINE_BUTTON_DELETE,

    /* Poly Bezier dialog */
    VECTOR_DLGSTR_POLYBEZIER_CAPTION,
    VECTOR_DLGSTR_POLYBEZIER_FRAME_POINTS,
    VECTOR_DLGSTR_POLYBEZIER_BUTTON_NEW,
    VECTOR_DLGSTR_POLYBEZIER_BUTTON_DELETE,

    /* Polygon dialog */
    VECTOR_DLGSTR_POLYGON_CAPTION,
    VECTOR_DLGSTR_POLYGON_FRAME_POINTS,
    VECTOR_DLGSTR_POLYGON_BUTTON_NEW,
    VECTOR_DLGSTR_POLYGON_BUTTON_DELETE,
    VECTOR_DLGSTR_POLYGON_FRAME_FILLMODE,
    VECTOR_DLGSTR_POLYGON_RADIO_ALTERNATE,
    VECTOR_DLGSTR_POLYGON_RADIO_WINDING,

    /* Ellipse dialog */
    VECTOR_DLGSTR_ELLIPSE_CAPTION,
    VECTOR_DLGSTR_ELLIPSE_FRAME_CENTER,
    VECTOR_DLGSTR_ELLIPSE_FRAME_RADIUS,

    /* Circle dialog */
    VECTOR_DLGSTR_CIRCLE_CAPTION,
    VECTOR_DLGSTR_CIRCLE_FRAME_CENTER,
    VECTOR_DLGSTR_CIRCLE_FRAME_RADIUS,

    /* Arc dialog */
    VECTOR_DLGSTR_ARC_CAPTION,
    VECTOR_DLGSTR_ARC_FRAME_CENTER,
    VECTOR_DLGSTR_ARC_FRAME_RADIUS,
    VECTOR_DLGSTR_ARC_FRAME_ANGLES,
    VECTOR_DLGSTR_ARC_STATIC_START,
    VECTOR_DLGSTR_ARC_STATIC_SWEEP,

    /* Text dialog */
    VECTOR_DLGSTR_TEXT_CAPTION,
    VECTOR_DLGSTR_TEXT_FRAME_POINT,
    VECTOR_DLGSTR_TEXT_FRAME_HORIZONTAL,
    VECTOR_DLGSTR_TEXT_FRAME_VERTICAL,
    VECTOR_DLGSTR_TEXT_CHECK_HITTESTRECTANGLE,
    VECTOR_DLGSTR_TEXT_FRAME_TEXT,
    VECTOR_DLGSTR_TEXT_FRAME_FONT,
    VECTOR_DLGSTR_TEXT_BUTTON_FONT,
    VECTOR_DLGSTR_TEXT_STATIC_CHARWIDTH,
    VECTOR_DLGSTR_TEXT_STATIC_CHARHEIGHT,

    /* Text alignment combo */
    VECTOR_DLGSTR_TEXT_LEFT,
    VECTOR_DLGSTR_TEXT_RIGHT,
    VECTOR_DLGSTR_TEXT_HCENTER,
    VECTOR_DLGSTR_TEXT_TOP,
    VECTOR_DLGSTR_TEXT_BOTTOM,
    VECTOR_DLGSTR_TEXT_VCENTER,

    /* Pie dialog */
    VECTOR_DLGSTR_PIE_CAPTION,
    VECTOR_DLGSTR_CHORD_CAPTION,
    VECTOR_DLGSTR_PIE_FRAME_CENTER,
    VECTOR_DLGSTR_PIE_FRAME_RADIUS,
    VECTOR_DLGSTR_PIE_FRAME_ANGLES,
    VECTOR_DLGSTR_PIE_STATIC_START,
    VECTOR_DLGSTR_PIE_STATIC_SWEEP,

    /* Poly Draw dialog */
    VECTOR_DLGSTR_POLYDRAW_CAPTION,
    VECTOR_DLGSTR_POLYDRAW_FRAME_POINTS,
    VECTOR_DLGSTR_POLYDRAW_BUTTON_NEW,
    VECTOR_DLGSTR_POLYDRAW_BUTTON_DELETE,
    VECTOR_DLGSTR_POLYDRAW_FRAME_TYPE,
    VECTOR_DLGSTR_POLYDRAW_CHECK_CLOSEFIGURE,

    /* Poly Draw types */
    VECTOR_DLGSTR_POLYDRAW_MOVETO,
    VECTOR_DLGSTR_POLYDRAW_LINETO,
    VECTOR_DLGSTR_POLYDRAW_BEZIERTO,

    /* Raster dialog */
    VECTOR_DLGSTR_RASTER_CAPTION,
    VECTOR_DLGSTR_RASTER_FRAME_POINT1,
    VECTOR_DLGSTR_RASTER_FRAME_POINT2,
    VECTOR_DLGSTR_RASTER_FRAME_BITMAP,

    /* Clone dialog */
    VECTOR_DLGSTR_CLONE_CAPTION,
    VECTOR_DLGSTR_CLONE_FRAME_POINT,
    VECTOR_DLGSTR_CLONE_FRAME_SCALE,
    VECTOR_DLGSTR_CLONE_FRAME_ROTATION,
    VECTOR_DLGSTR_CLONE_FRAME_ATTRIBUTES,
    VECTOR_DLGSTR_CLONE_CHECK_USEPEN,
    VECTOR_DLGSTR_CLONE_CHECK_USEBRUSH,
    VECTOR_DLGSTR_CLONE_CHECK_USEPOINTASORIGIN,
    VECTOR_DLGSTR_CLONE_FRAME_GROUP,

    /* Elliptical arc dialog */
    VECTOR_DLGSTR_ELLIPTICALARC_CAPTION,
    VECTOR_DLGSTR_ELLIPTICALARC_FRAME_CENTER,
    VECTOR_DLGSTR_ELLIPTICALARC_FRAME_ANGLES,
    VECTOR_DLGSTR_ELLIPTICALARC_STATIC_START,
    VECTOR_DLGSTR_ELLIPTICALARC_STATIC_SWEEP,
    VECTOR_DLGSTR_ELLIPTICALARC_FRAME_RADIUS,

    /* Messages */
    VECTOR_DLGSTR_VECTOR_IS_LOCKED,
    VECTOR_DLGSTR_VECTOR_NOT_ENABLED,
    VECTOR_DLGSTR_LAYER_NOT_FOUND,
    VECTOR_DLGSTR_LAYER_ALREADY_EXISTS,
    VECTOR_DLGSTR_GROUP_NOT_FOUND,
    VECTOR_DLGSTR_GROUP_ALREADY_EXISTS,
    VECTOR_DLGSTR_NO_MEMORY,
    VECTOR_DLGSTR_DELETELAYERQUERY,
    VECTOR_DLGSTR_DELETEGROUPQUERY,
    VECTOR_DLGSTR_CANNOTFINISHOPERATION,
    VECTOR_DLGSTR_CANNOTLOADBITMAP,

    VECTOR_DLGSTR_MAX
};

#define VECTOR_DLGSTR_CHORD_FRAME_CENTER VECTOR_DLGSTR_PIE_FRAME_CENTER
#define VECTOR_DLGSTR_CHORD_FRAME_RADIUS VECTOR_DLGSTR_PIE_FRAME_RADIUS
#define VECTOR_DLGSTR_CHORD_FRAME_ANGLES VECTOR_DLGSTR_PIE_FRAME_ANGLES
#define VECTOR_DLGSTR_CHORD_STATIC_START VECTOR_DLGSTR_PIE_STATIC_START
#define VECTOR_DLGSTR_CHORD_STATIC_SWEEP VECTOR_DLGSTR_PIE_STATIC_SWEEP

/* functions */
L_INT EXT_FUNCTION L_VecDlgRotate(HWND hWnd, pVECTORHANDLE pVector, pVECTORPOINT pRotation, const pVECTORPOINT pOrigin, L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgScale(HWND hWnd, pVECTORHANDLE pVector, pVECTORPOINT pScale, const pVECTORPOINT pOrigin, L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgTranslate(HWND hWnd, pVECTORHANDLE pVector, pVECTORPOINT pTranslation, L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgCamera(HWND hWnd, pVECTORHANDLE pVector, pVECTORCAMERA pCamera, L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgRender(HWND hWnd,
                                  pVECTORHANDLE pVector,
                                  L_BOOL L_FAR* pbUseLights,
                                  COLORREF L_FAR* pColorAmbient,
                                  L_INT L_FAR* pnPolygonMode,
                                  L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgViewMode(HWND hWnd, pVECTORHANDLE pVector, L_INT L_FAR* pnViewMode, L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgHitTest(HWND hWnd, pVECTORHANDLE pVector, pVECTORHITTEST pHitTest, L_UINT32 dwFlags);

L_INT EXT_FUNCTION L_VecDlgEditAllLayers(HWND hWnd, pVECTORHANDLE pVector, L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgNewLayer(HWND hWnd,
                                    pVECTORHANDLE pVector,
                                    pVECTORLAYERDESC pLayerDesc,
                                    pVECTORLAYER pLayer,
                                    L_BOOL L_FAR* pbActiveLayer,
                                    L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgEditLayer(HWND hWnd,
                                     pVECTORHANDLE pVector,
                                     pVECTORLAYER pLayer,
                                     pVECTORLAYERDESC pLayerDesc,
                                     L_BOOL L_FAR* pbActiveLayer,
                                     L_UINT32 dwFlags);

L_INT EXT_FUNCTION L_VecDlgEditAllGroups(HWND hWnd, pVECTORHANDLE pVector, L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgNewGroup(HWND hWnd, pVECTORHANDLE pVector, pVECTORGROUPDESC pGroupDesc, pVECTORGROUP pGroup, L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgEditGroup(HWND hWnd, pVECTORHANDLE pVector, pVECTORGROUP pGroup, pVECTORGROUPDESC pGroupDesc, L_UINT32 dwFlags);

L_INT EXT_FUNCTION L_VecDlgNewObject(HWND hWnd,
                                     pVECTORHANDLE pVector,
                                     const pVECTORLAYER pLayer,
                                     L_INT nType,
                                     L_VOID L_FAR* pObjectDesc,
                                     pVECTOROBJECT,
                                     L_UINT32 dwFlags);
L_INT EXT_FUNCTION
L_VecDlgEditObject(HWND hWnd, pVECTORHANDLE pVector, const pVECTOROBJECT pObject, L_INT nType, L_VOID L_FAR* pObjectDesc, L_UINT32 dwFlags);
L_INT EXT_FUNCTION L_VecDlgObjectAttributes(HWND hWnd,
                                            pVECTORHANDLE pVector,
                                            const pVECTOROBJECT pObject,
                                            L_BOOL* pbSelected,
                                            L_INT* pnROP,
                                            pVECTORPEN pPen,
                                            pVECTORBRUSH pBrush,
                                            pVECTORFONT pFont,
                                            L_UINT32 dwFlags);

L_INT EXT_FUNCTION L_VecDlgGetStringLen(L_UINT32 uString, L_UINT L_FAR* puLen);
L_INT EXT_FUNCTION L_VecDlgGetString(L_UINT32 uString, L_CHAR L_FAR* pszString);
L_INT EXT_FUNCTION L_VecDlgSetString(L_UINT32 uString, const L_CHAR L_FAR* pszString);
HFONT EXT_FUNCTION L_VecDlgSetFont(HFONT hFont);

#undef _HEADER_ENTRY_
#include "ltpck.h"

#endif /* _LVDLG_H_ */
