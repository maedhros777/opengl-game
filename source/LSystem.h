#ifndef LSYSTEM_H
#define LSYSTEM_H
#include <string>
#include <vector>
#include <cmath>
#include "Point.h"
#include "Vector.h"
#include "Debug.h"

const int LSYSTEM_RECURSIVE_DEPTH = 4;
const int MAX_LSYSTEM_DEPTH = 10;
const float LSYSTEM_ANGLE = 24.8;
const float LSYSTEM_BRANCH_SIZE = 0.04;

enum GrammarSymbol //TODO: add support for parametric systems and more advanced symbols
{
    A = 1, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    LEFT_BRACKET, RIGHT_BRACKET, PLUS, MINUS, NOP_X, NOP_Y, NOP_Z
};

char toChar(GrammarSymbol);

typedef GrammarSymbol Axiom;

struct Branch
{
    Point3f pos;
    float angle;
};

class LSystem
{
private:
    void calcVertices();
protected:
    int recursive_depth;
    float angle, branch_size;
    Axiom axiom;
    std::vector<GrammarSymbol> parsedSymbols;
    std::vector<Point3f> vertices;
public:
    LSystem(int depth = LSYSTEM_RECURSIVE_DEPTH, Axiom _axiom = F, float _angle = LSYSTEM_ANGLE, float size = LSYSTEM_BRANCH_SIZE)
                : recursive_depth(depth), angle(LSYSTEM_ANGLE), branch_size(LSYSTEM_BRANCH_SIZE), axiom(_axiom)  { }
    void parse();
    void render() const;
    virtual void reproduce(GrammarSymbol, std::vector<GrammarSymbol> &) = 0;

    int getRecursiveDepth() const
    {
        return recursive_depth;
    }

    float getAngle() const
    {
        return angle;
    }

    void setAngle(float _angle)
    {
        angle = _angle;
    }
};

#define LSYSTEM(name, axiom) class name : public LSystem { \
    public: name(int depth = LSYSTEM_RECURSIVE_DEPTH) : LSystem(depth, axiom)  { } \
    void reproduce(GrammarSymbol, std::vector<GrammarSymbol> &); \
}

#define CUSTOM_LSYSTEM(name, axiom, _angle, size, depth) class name : public LSystem { \
    public: name(int _depth = depth) : LSystem(_depth, axiom, _angle, size)  { } \
    void reproduce(GrammarSymbol, std::vector<GrammarSymbol> &); \
}

#define LSYSTEM_IMPLEMENT(name) void name::reproduce(GrammarSymbol symbol, std::vector<GrammarSymbol> &vec)

#define LSYSTEM_BEGIN_IMPLEMENT unsigned int i; switch (symbol) {

#define LSYSTEM_REPLACE(symbol, array) case symbol: for (i = 0; i < sizeof(array) / sizeof(GrammarSymbol); ++i) \
    vec.push_back(array[i]); break

#define LSYSTEM_END_IMPLEMENT default: vec.push_back(symbol); }

LSYSTEM(SimplePlant, F);
CUSTOM_LSYSTEM(Plant, NOP_X, LSYSTEM_ANGLE, 0.07, 5);

/* 	NOTE: these macros are used to define new L-systems quickly and efficiently.
	The first step in defining a new subclass of LSystem is to use either the LSYSTEM() or CUSTOM_LSYSTEM() macros.
	More steps must be followed in the implementation file.
*/

#endif
