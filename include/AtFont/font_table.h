#if !( defined(__ATLIB_FONT_H) || defined(__DEBUG__) )
#error "ATLIB ERROR: Do not use \"font_internal.h\" directly; Use \"AtFont.h\" instead"
#endif

#ifndef __ATLIB_FONT_TABLE_H
#define __ATLIB_FONT_TABLE_H

#include "Atlib/bezier.h"
#include "Atlib/types.h"
#include "Atlib/utils.h"

typedef struct {
    u8 tableTag[4];
    u32 checksum;
    u32 offset;
    u32 length;
} TableRecord;

typedef struct {
    u32 sfntVersion;
    u16 numTables;
    u16 searchRange;
    u16 entrySelector;
    u16 rangeShift;
    TableRecord * recs;
} TableDirectory;

typedef struct {
    u16 majorVersion;
    u16 minorVersion;
    i32 fontRevision;
    u32 checksumAdjustment;
    u32 magicNumber;
    u16 flags;
    u16 unitsPerEm;
    i64 created;
    i64 modified;
    i16 xMin;
    i16 yMin;
    i16 xMax;
    i16 yMax;
    u16 macStyle;
    u16 lowestRecPPEM;
    i16 fontDirectionHint;
    i16 indexToLocFormat;
    i16 glyphDataFormat;
} Head;

typedef struct {
    u32 version;
    u16 numGlyphs;
    u16 maxPoints;
    u16 maxContours;
    u16 maxCompositPoints;
    u16 maxCompositContours;
    u16 maxZones;
    u16 maxTwilightPoints;
    u16 maxStorage;
    u16 maxFunctionDefs;
    u16 maxInstructionDefs;
    u16 maxStackElements;
    u16 maxSizeOfInstructions;
    u16 maxComponentElements;
    u16 maxComponentDepth;
} Maxp;

typedef struct {
    u16 advanceWidth;
    i16 lsb;
} LongHorMetric;

typedef struct {
    LongHorMetric * hMetrics;
    i16 * leftSideBearings;
} Hmtx;

typedef struct {
    u16 majorVersion;
    u16 minorVersion;
    i16 ascender;
    i16 descender;
    i16 lineGap;
    u16 advanceWidthMax;
    i16 minLeftSideBearing;
    i16 minRightSideBearing;
    i16 xMaxExtent;
    i16 caretSlopRise;
    i16 caretSlopRun;
    i16 caretOffset;
    i16 metricDataFormat;
    u16 numberOfHMetrics;
} Hhea;

typedef struct {
    u32 * offset;
} Loca;

typedef struct {
    u16 platformID; /* Must always be 0 */
    u16 encodingID; /* Must always be 3 */
    u32 subtableOffset; /* Local offset; From beginning of CMAP table */
} CmapEncodingRecord;

typedef struct {
    u16 format;
    u16 length;
    u16 language;
    u16 segCountX2;
    u16 searchRange;
    u16 entrySelector;
    u16 rangeShift;
    u16 * endCodes;
    u16 __reserve; /* Must always be 0 */
    u16 * startCodes;
    i16 * idDeltas;
    u16 * idRangeOffsets;
    u16 * glyfIdArrays;
} CmapSubtable4;

typedef struct {
    u16 version;
    u16 numTables;
    CmapEncodingRecord * encodingRecords;
    CmapSubtable4 subtable;
} Cmap;

typedef struct {
    i16 numberOfContours;
    i16 xMin;
    i16 yMin;
    i16 xMax;
    i16 yMax;
} GlyfHeader;

typedef struct {
    GlyfHeader header;
    u16 * endPtsOfContours;
    u16 instructionLength;
    u8 * instructions;
    u8 * flags;
    i16 * xCoordinates;
    i16 * yCoordinates;
} SimpleGlyf;

typedef struct {
    u16 flags;
    u16 glyphIndex;
    i16 argument1;
    i16 argument2;
    double xscale;
    double yscale;
    double scale_1;
    double scale_2;
} ComplexGlyfComponent;

typedef struct {
    GlyfHeader header;
    ComplexGlyfComponent * components;
} ComplexGlyf;

typedef union {
    GlyfHeader header;
    SimpleGlyf simple;
    ComplexGlyf complex;
} Glyf;
CREATE_OPTIONAL(Glyf);

typedef enum {
    ON_CURVE_POINT = 0x01,
    X_SHORT_VECTOR = 0x02,
    Y_SHORT_VECTOR = 0x04,
    REPEAT_FLAG = 0x08,
    X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR = 0x10,
    Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR = 0x20,
    OVERLAP_SAMPLE = 0x40,
    SIMPLE_FLAG_RESERVED = 0x80,
} SimpleGlyphFlags;

typedef enum {
    ARG_1_AND_2_ARE_WORDS = 0x0001,
    ARGS_ARE_XY_VALUES = 0x0002,
    ROUND_XY_TO_GRID = 0x0004,
    WE_HAVE_A_SCALE = 0x0008,
    MORE_COMPONENTS = 0x0020,
    WE_HAVE_AN_X_AND_Y_SCALE = 0x0040,
    WE_HAVE_A_TWO_BY_TWO = 0x0080,
    WE_HAVE_INSTRUCTIONS = 0x0100,
    USE_MY_METRICS = 0x0200,
    OVERLAP_COMPOUND = 0x0400,
    SCALED_COMPONENT_OFFSET = 0x0800,
    UNSCALED_COMPONENT_OFFSET = 0x1000,
    COMPOUND_FLAG_RESERVED = 0xE010
} CompoundGlyphFlags;

typedef struct __font {
    TableDirectory dir;
    Cmap cmap;
    Head head;
    Hhea hhea;
    Maxp maxp;
    Loca loca;
    Hmtx hmtx;
    Glyf_optional_t * glyf;
    cubic_bezier_t curve;
} font_t;

#endif
