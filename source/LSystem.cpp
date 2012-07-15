#include "LSystem.h"

char toChar(GrammarSymbol gs)
{
    char c;

    switch (gs)
    {
    case G:
        c = 'G';
        break;
    case F:
        c = 'F';
        break;
    case LEFT_BRACKET:
        c = '[';
        break;
    case RIGHT_BRACKET:
        c = ']';
        break;
    case PLUS:
        c = '+';
        break;
    case MINUS:
        c = '-';
        break;
    default:
        c = '?';
        break;
    }

    return c;
}

void LSystem::parse()
{
    int size, i;
    parsedSymbols.push_back(axiom);
    std::vector<GrammarSymbol> newsymbols;

    for (i = 0; i < recursive_depth; ++i)
    {
        newsymbols.clear();
        size = parsedSymbols.size();

        for (int j = 0; j < size; ++j)
            reproduce(parsedSymbols[j], newsymbols);

        parsedSymbols = newsymbols;
    }

    calcVertices();
}

void LSystem::calcVertices()
{
    int size = parsedSymbols.size();
    Branch branch, oldbranch;
    branch.angle = 0.0;
    std::vector<Branch> stack;

    for (int i = 0; i < size; ++i)
    {
        switch (parsedSymbols[i])
        {
        case LEFT_BRACKET:
            stack.push_back(branch);
            break;
        case RIGHT_BRACKET:
            branch = stack.back();
            stack.pop_back();
            break;
        case PLUS:
            branch.angle -= angle;
            break;
        case MINUS:
            branch.angle += angle;
            break;
        case NOP_X:
        case NOP_Y:
        case NOP_Z:
            break;
        default:
            oldbranch = branch;
            branch.pos.setX(branch.pos.getX() + branch_size * cos(90.0 - branch.angle));
            branch.pos.setY(branch.pos.getY() + branch_size * sin(90.0 - branch.angle));
            vertices.push_back(oldbranch.pos);
            vertices.push_back(branch.pos);
        }
    }
}

void LSystem::render() const
{
    glColor3f(0.0, 0.2, 0.0);
    int size = vertices.size();
    Point3f vert;

    glPushMatrix();
    glRotatef(-angle, 0.0, 0.0, 1.0);
    glBegin(GL_LINES);

    for (int i = 0; i < size; ++i)
    {
        vert = vertices[i];
        glVertex3f(vert.getX(), vert.getY(), vert.getZ());
    }

    glEnd();
    glPopMatrix();
}

LSYSTEM_IMPLEMENT(SimplePlant)
{
    GrammarSymbol f_replace[] = { G, LEFT_BRACKET, PLUS, F, RIGHT_BRACKET, LEFT_BRACKET, MINUS, F, RIGHT_BRACKET, G, F, };
    GrammarSymbol g_replace[] = { G, G };
    LSYSTEM_BEGIN_IMPLEMENT
    LSYSTEM_REPLACE(F, f_replace);
    LSYSTEM_REPLACE(G, g_replace);
    LSYSTEM_END_IMPLEMENT
}

LSYSTEM_IMPLEMENT(Plant)
{
    GrammarSymbol x_replace[] = { F, MINUS, LEFT_BRACKET, LEFT_BRACKET, NOP_X, RIGHT_BRACKET, PLUS, NOP_X,
        RIGHT_BRACKET, PLUS, F, LEFT_BRACKET, PLUS, F, NOP_X, RIGHT_BRACKET, MINUS, NOP_X };
    GrammarSymbol f_replace[] = { F, F };
    LSYSTEM_BEGIN_IMPLEMENT
    LSYSTEM_REPLACE(NOP_X, x_replace);
    LSYSTEM_REPLACE(F, f_replace);
    LSYSTEM_END_IMPLEMENT
}

/*	NOTE: here is the implementation for the Plant and SimplePlant example subclasses.
	Implementation should consist of:
		The LSYSTEM_IMPLEMENT() macro, with the name of the subclass as a parameter
		Declaration of one or more GrammarSymbol[] arrays, each describing the substitutions for a certain grammar symbol
		The macro LSYSTEM_BEGIN_IMPLEMENT
		A series of calls to LSYSTEM_REPLACE(), with a grammar symbol and its corresponding substitution array as parameters
		The macro LSYSTEM_END_IMPLEMENT
*/
