var X = 0, Y = 1, Z = 2, W = 3;

var grid = [
    -1, 0, 0, 1,
    +1, 0, 0, 1,
    0, -1, 0, 1,
    0, +1, 0, 1
];

function clear_graph() {
    "use strict"

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glColor4f(1, 1, 1, 1.000);

    glClearColor(0, 0, 0, 0.000);
    glClear(GL_COLOR_BUFFER_BIT);

    glVertexPointer(2, GL_FLOAT, 4 * 4, grid);
    glEnableClientState(GL_VERTEX_ARRAY);

    glLineWidth(1.0);
    glDisableClientState(GL_COLOR_ARRAY);

    glDrawArrays(GL_LINES, 0, 2); /* x-axis */
    glDrawArrays(GL_LINES, 2, 2); /* y-axis */
    return;
}

var inverse_zoom = 1.0; /* graph magnification = 1 / inverse_zoom */

var f_x = "";
var g_x = "";
var h_x = "";
var i_x = "";
var j_x = "";
var k_x = "";
var l_x = "";
var m_x = "";

function f(x) {
    "use strict";
    return eval(f_x);
}
function g(x) {
    "use strict";
    return eval(g_x);
}
function h(x) {
    "use strict";
    return eval(h_x);
}
function ii(x) {
    "use strict";
    return eval(i_x);
}
function j(x) {
    "use strict";
    return eval(j_x);
}
function k(x) {
    "use strict";
    return eval(k_x);
}
function l(x) {
    "use strict";
    return eval(l_x);
}
function m(x) {
    "use strict";
    return eval(m_x);
}

/*
 * alias functions to help simplify the amount of typing user has to do in the
 * definitions of each function
 */

var pi = Math.PI;
var e = Math.E;

function sin(x) {
    "use strict";
    return Math.sin(x);
}
function cos(x) {
    "use strict";
    return Math.cos(x);
}
function tan(x) {
    "use strict";
    return Math.tan(x);
}

function csc(x) {
    "use strict";
    return (1 / sin(x));
}
function sec(x) {
    "use strict";
    return (1 / cos(x));
}
function cot(x) {
    "use strict";
    return (1 / tan(x));
}

function arcsin(x) {
    "use strict";
    return Math.asin(x);
}
function arccos(x) {
    "use strict";
    return Math.acos(x);
}
function arctan(x) {
    "use strict";
    return Math.atan(x);
}

function sqrt(x) {
    "use strict";
    return Math.sqrt(x);
}
function cbrt(x) {
    "use strict";
    return Math.cbrt(x);
}
function pow(base, power) {
    "use strict";
    return Math.pow(base, power);
}

function abs(x) {
    "use strict";
    return Math.abs(x);
}

function graph_all() {
    "use strict";
    var x = -1.0 / inverse_zoom;
    var i = 0;
    var raster_pitch = 512;
    var coords = 2;
    var stride = 4 * 4; /* coords_per_vertex * sizeof(GLfloat) */
    var vertex_buffer = [];

    while (i < raster_pitch) {
        vertex_buffer[4*i + X] = x;
        x += 2.0 / raster_pitch;
        vertex_buffer[4*i + Z] = 0.0;
        vertex_buffer[4*i + W] = inverse_zoom;
        i += 1;
    }

    clear_graph();
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glLineWidth(1.0);

    f_x = document.getElementById("f_x").value;
    g_x = document.getElementById("g_x").value;
    h_x = document.getElementById("h_x").value;
    i_x = document.getElementById("i_x").value;
    j_x = document.getElementById("j_x").value;
    k_x = document.getElementById("k_x").value;
    l_x = document.getElementById("l_x").value;
    m_x = document.getElementById("m_x").value;

    if (f_x) {
        i = 0;
        x = -1.0 / inverse_zoom;
        while (i < raster_pitch) {
            vertex_buffer[4*i + Y] = f(x);
            x += 2.0 / raster_pitch;
            i += 1;
        }
        glColor4f(1, 0, 0, 1); /* red (primary) */
        glVertexPointer(coords, GL_FLOAT, stride, vertex_buffer);
        glDrawArrays(GL_LINE_STRIP, 0, i);
    }
    if (g_x) {
        i = 0;
        x = -1.0 / inverse_zoom;
        while (i < raster_pitch) {
            vertex_buffer[4*i + Y] = g(x);
            x += 2.0 / raster_pitch;
            i += 1;
        }
        glColor4f(0, 0, 1, 1); /* blue (primary) */
        glVertexPointer(coords, GL_FLOAT, stride, vertex_buffer);
        glDrawArrays(GL_LINE_STRIP, 0, i);
    }
    if (h_x) {
        i = 0;
        x = -1.0 / inverse_zoom;
        while (i < raster_pitch) {
            vertex_buffer[4*i + Y] = h(x);
            x += 2.0 / raster_pitch;
            i += 1;
        }
        glColor4f(1, 1, 0, 1); /* yellow (primary subtractive pigments) */
        glVertexPointer(coords, GL_FLOAT, stride, vertex_buffer);
        glDrawArrays(GL_LINE_STRIP, 0, i);
    }
    if (i_x) {
        i = 0;
        x = -1.0 / inverse_zoom;
        while (i < raster_pitch) {
            vertex_buffer[4*i + Y] = ii(x);
            x += 2.0 / raster_pitch;
            i += 1;
        }
        glColor4f(0, 1, 0, 1); /* green (primary, secondary subtractive) */
        glVertexPointer(coords, GL_FLOAT, stride, vertex_buffer);
        glDrawArrays(GL_LINE_STRIP, 0, i);
    }
    if (j_x) {
        i = 0;
        x = -1.0 / inverse_zoom;
        while (i < raster_pitch) {
            vertex_buffer[4*i + Y] = j(x);
            x += 2.0 / raster_pitch;
            i += 1;
        }
        glColor4f(1, 0, 1, 1); /* magenta (secondary) */
        glVertexPointer(coords, GL_FLOAT, stride, vertex_buffer);
        glDrawArrays(GL_LINE_STRIP, 0, i);
    }
    if (k_x) {
        i = 0;
        x = -1.0 / inverse_zoom;
        while (i < raster_pitch) {
            vertex_buffer[4*i + Y] = k(x);
            x += 2.0 / raster_pitch;
            i += 1;
        }
        glColor4f(0, 1, 1, 1); /* cyan (secondary, tertiary subtractive) */
        glVertexPointer(coords, GL_FLOAT, stride, vertex_buffer);
        glDrawArrays(GL_LINE_STRIP, 0, i);
    }
    if (l_x) {
        i = 0;
        x = -1.0 / inverse_zoom;
        while (i < raster_pitch) {
            vertex_buffer[4*i + Y] = l(x);
            x += 2.0 / raster_pitch;
            i += 1;
        }
        glColor4f(1, 0.5, 0, 1); /* orange (tertiary, secondary subtractive) */
        glVertexPointer(coords, GL_FLOAT, stride, vertex_buffer);
        glDrawArrays(GL_LINE_STRIP, 0, i);
    }
    if (m_x) {
        i = 0;
        x = -1.0 / inverse_zoom;
        while (i < raster_pitch) {
            vertex_buffer[4*i + Y] = m(x);
            x += 2.0 / raster_pitch;
            i += 1;
        }
        glColor4f(0.5, 0, 1.0, 1); /* indigo (tertiary) */
        glVertexPointer(coords, GL_FLOAT, stride, vertex_buffer);
        glDrawArrays(GL_LINE_STRIP, 0, i);
    }
    return;
}

function main_GL() {
    "use strict";
    var error_code;

    if (GL_get_context(document, "GL_canvas") === null) {
        alert("Failed to initialize WebGL.");
        return;
    }

    clear_graph();
    do {
        error_code = glGetError();
        console.log("OpenGL error status:  " + error_code);
    } while (error_code !== GL_NO_ERROR);
    return;
}
