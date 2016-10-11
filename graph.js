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

function f(x) {
    "use strict";
    return eval(f_x);
}

function graph_all() {
    "use strict";
    var x = -1.0 / inverse_zoom;
    var i = 0;
    var vertex_buffer = [];

    clear_graph();
    glEnableClientState(GL_VERTEX_ARRAY);
    glLineWidth(1.0);

    f_x = document.getElementById("f_x").value;
    if (f_x) {
        while (i < 512 && x <= +1.0) {
            vertex_buffer[4*i + X] = x;
            vertex_buffer[4*i + Y] = f(x);
            vertex_buffer[4*i + Z] = 0.0;
            vertex_buffer[4*i + W] = inverse_zoom;

            x += 1.0 / 256;
            i += 1;
        }
        glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
        glVertexPointer(4, GL_FLOAT, 0, vertex_buffer);
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
