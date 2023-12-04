#include "jpgfuncs.h"

using namespace std;

void UPDATE_SIMULATION(vector<GLubyte> &pixs, int dwidth, int dheight, array<function<void(vector<GLubyte>&, int i, PGInfo&, GLubyte, array<GLubyte, 16>)>, 16> &powderFuncs, bool &oddf, array<GLubyte, 16>& densities) {

    static PGInfo info;

    info.targetOddBit = info.oddFrame ? 0b10000000 : 0b00000000;
    info.nonTargetOddBit = info.oddFrame ? 0b00000000 : 0b10000000;

    //cout << "up " << (oddFrame ? "t" : "f") << '\n';

    for(int y = dheight-1; y > 0; y--) {
        for(int x = 0; x < dwidth; x++) {
            int i = y * dwidth + x;

            GLubyte colorBitsHere = pixs[i] & info.COLOR_BITS;

            if(colorBitsHere != 0b00000000 && y > 0 && (pixs[i] & info.ODD_BIT) == info.targetOddBit
               && i != dwidth*(dheight-1)) {
                powderFuncs[colorBitsHere](pixs, i, info, colorBitsHere, densities);
            }
        }
    }
    info.oddFrame = !info.oddFrame;
    oddf = info.oddFrame;
}

void draw_at_cursor(double xpos, double ypos, vector<GLubyte> &pixs, int wheight, double ratio, int dwidth, int dheight, int selectedColor, bool oddf) {
    int yp = wheight - static_cast<int>(ypos);

    int dx, dy;
    dx = static_cast<int>(xpos*ratio);
    dy = static_cast<int>(yp*ratio);

    int index = dy * dwidth + dx;

    static bool isoddpix = false;

    GLubyte theByte = selectedColor;
    if(isoddpix) {
        theByte |= PGInfo::LIQUID_TRAV_LEFT_BIT;
    }
    if(index != dwidth*(dheight-1)) {
        pixs[index] = theByte;
    }
    isoddpix = !isoddpix;
}

void update_time(double &deltat, double &lastframe) {
    double current_frame = glfwGetTime();
    deltat = current_frame - lastframe;
    lastframe = current_frame;
}