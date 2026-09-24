/**
 * Computer Graphics - Project - Solar System
 *
 **/
#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <chrono>
#include <thread>

// Toggle for animation
bool startAnimation = false;

// SUN

float sunAngle = 0.0f; // Angle for Sun's rotation
float sunOrbitAngle = 0.0f; // Angle for Sun's orbit

// STARS

const int numStars = 100; // Number of stars
float stars[numStars][3]; // Array to store star positions

// EARTH

// Define the vertices for the Earth's surface
std::vector<std::vector<float>> earthVertices;

// Define green and blue regions for the Earth's checkerboard-style surface
std::vector<std::vector<int>> landPolygons;
std::vector<std::vector<int>> waterPolygons;

float earthOrbitAngle = 0.0f;
float earthOrbitRadius = 8.0f;

// JUPITER

// In the initialization part (init function), define colors for Jupiter bands
std::vector<std::vector<float>> jupiterColors;
// Define vertices for Jupiter
std::vector<std::vector<float>> jupiterVertices;

const int numBands = 10; // Number of distinct color bands on Jupiter
const int numRings = 50;    // Number of rings to approximate the sphere
const int numSegments = 50; // Number of segments to approximate the sphere

float jupiterOrbitRadius = 5.0f; // Adjust this value for Jupiter's orbit distance

// ISS

bool issClicked = false; // Flag to indicate if ISS is clicked

float issX = 0.0f; // X-coordinate of ISS
float issY = 0.0f; // Y-coordinate of ISS
float issSpeed = 0.1f; // Speed of ISS movement
float issWidth = 0.4f;
float issHeight = 0.2f;
float solarPanelWidth = 0.22f * 2; // Width of a single panel multiplied by 2 (since it's symmetric)
float solarPanelHeight = 0.14f * 2; // Height of a single panel multiplied by 2 (since it's symmetric)

// ISS 2D
float issXPosition = 2.8f; // Starting position on the right

// Text Effect

// Define global variables to control text animation
bool textAnimationInProgress = false; // Control whether text animation is ongoing
std::string displayedText = "";      // Currently displayed text
int currentTextIndex = 0;            // Index of the current character being displayed
bool cursorVisible = true;          // Control cursor visibility
auto lastCharacterTime = std::chrono::steady_clock::now();

// Define a scaling factor for text size
float textSize = 0.15f;

// Listener for keyboard clicks
void keyboard(unsigned char key, int x, int y) {
    // Listen for 's' letter
    if (key == 's' || key == 'S') {
        // If animation running, make it stop
        if (startAnimation) {
            startAnimation = false;
        }
        // otherwise make it running
        else {
            startAnimation = true;
        }
    }
}

// Listener for mouse clicks
void mouse(int button, int state, int x, int y) {
    // Listen for the left button
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // If animation running, make it stop
        if (startAnimation) {
            startAnimation = false;
        }
        // otherwise make it running
        else {
            startAnimation = true;
        }
    }
}

// Generate Vertices to form a sphere (Jupiter)
void generateSphereVertices(float radius, int numRings, int numSegments) {
    for (int i = 0; i <= numRings; ++i) {
        float theta = i * M_PI / numRings;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int j = 0; j <= numSegments; ++j) {
            float phi = j * 2 * M_PI / numSegments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = radius * cosPhi * sinTheta;
            float y = radius * cosTheta;
            float z = radius * sinPhi * sinTheta;

            jupiterVertices.push_back({x, y, z});
        }
    }
}

// Init Sun
void initSun() {}

// Init Stars
void initStars() {
    // Initialize random seed
    std::srand(std::time(nullptr));

    // Generate random star positions
    for (int i = 0; i < numStars; ++i) {
        stars[i][0] = static_cast<float>(std::rand() % 2400 - 1200) / 100.0f; // X position
        stars[i][1] = static_cast<float>(std::rand() % 1600 - 800) / 100.0f;  // Y position
        stars[i][2] = static_cast<float>(std::rand() % 200 - 100) / 100.0f;  // Z position
    }
}

// Init Vega
void initVega() {}

// Init Earth
void initEarth() {
    // Create the Earth's surface vertices
    for (int lat = -90; lat <= 90; lat += 10) {
        for (int lon = -180; lon <= 180; lon += 10) {
            float x = sin(lon * M_PI / 180) * cos(lat * M_PI / 180);
            float y = sin(lat * M_PI / 180);
            float z = cos(lon * M_PI / 180) * cos(lat * M_PI / 180);

            earthVertices.push_back({x, y, z});
        }
    }

    // Define green and blue regions for the Earth's checkerboard-style surface
    for (int lat = 0; lat < 18; ++lat) {
        for (int lon = 0; lon < 36; ++lon) {
            int v0 = lat * 37 + lon;
            int v1 = v0 + 1;
            int v2 = (lat + 1) * 37 + lon;
            int v3 = v2 + 1;

            // Alternate between green and blue surface regions
            if ((lat + lon) % 2 == 0) {
                landPolygons.push_back({v0, v2, v1});
                landPolygons.push_back({v1, v2, v3});
            } else {
                waterPolygons.push_back({v0, v2, v1});
                waterPolygons.push_back({v1, v2, v3});
            }
        }
    }
}

// Init Jupiter
void initJupiter() {
    // Generate vertices for Jupiter's sphere
    generateSphereVertices(2.0f, numRings, numSegments);

    // Define colors for Jupiter bands
    for (int i = 0; i < numRings / 2; ++i) {
        jupiterColors.push_back({0.9f, 0.7f, 0.3f}); // Light brown
        jupiterColors.push_back({0.8f, 0.6f, 0.2f}); // Dark brown
    }
}

// Initialize
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set the background color to black
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering

    initSun();
    initStars();
    initVega();
    initEarth();
    initJupiter();

}

// Function to render text
void renderText(const char* text) {
    glPushMatrix();

    if (startAnimation) {
        textAnimationInProgress = true;
        cursorVisible = true;
    }

    // Check if text animation is ongoing
    if (textAnimationInProgress) {
        // Calculate the time interval between character appearances
        auto currentTime = std::chrono::steady_clock::now();
        auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCharacterTime).count();

        // If enough time has passed, display the next character
        if (timeDiff >= 200) { // Adjust the delay (100 milliseconds) as needed
            // Display the next character and update the index
            if (currentTextIndex < strlen(text)) {
                displayedText += text[currentTextIndex];
                currentTextIndex++;
            }
            // Set the time for the appearance of the next character
            lastCharacterTime = currentTime;
        }

        // If all text is displayed, stop animation
        if (currentTextIndex >= strlen(text)) {
            textAnimationInProgress = false;
        }
    }

    // Render the displayed text
    glPushMatrix();
    // Set up orthographic projection for 2D text rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Scale the text to the desired size
    glScalef(textSize, textSize, 1.0f);

    // Adjust the position to be slightly above the bottom
    glTranslatef(10.0f, 500.0f, 0.0f); // Adjust the Y-coordinate as needed

    // Render the displayed text
    for (size_t i = 0; i < displayedText.length(); i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, displayedText[i]);
    }

    // Blinking underscore
    if (textAnimationInProgress) {
        glColor3f(1.0f, 1.0f, 1.0f); // Set underscore color (black)
        glBegin(GL_LINES);
        glVertex2f((displayedText.length() - 0.5) * 15, 10); // Adjust underscore position as needed
        glVertex2f((displayedText.length() + 0.5) * 15, 10); // Adjust underscore position as needed
        glEnd();
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopMatrix();
}

void drawISSThree() {
    // Apply a horizontal rotation to the ISS
    static float rotationAngle = 0.0f; // Initialize the rotation angle

    if (startAnimation) {
        rotationAngle += 0.5f; // Adjust the rotation speed as needed
    }

    glPushMatrix();
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // Rotate around the Y-axis
    glTranslatef(-1.5f, 1.0f, 0.0f);
    // Rotate the ISS to make it visible
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);

    // Set the color for the ISS (e.g., gray)
    glColor3f(0.8f, 0.8f, 0.8f);

    // Draw the main body of the ISS (a cylinder)
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluCylinder(quadric, 0.1, 0.1, 0.4, 20, 20);
    gluDeleteQuadric(quadric);

    // Draw the solar panels (rectangular prisms)
    glColor3f(0.6f, 0.6f, 0.6f); // Dark gray color for solar panels

    // Draw the first solar panel
    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0.0f);
    glScalef(0.4f, 0.02f, 0.2f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw the second solar panel
    glPushMatrix();
    glTranslatef(-0.15f, 0.0f, 0.0f);
    glScalef(0.4f, 0.02f, 0.2f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw the third solar panel
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.15f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.4f, 0.02f, 0.2f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Position and rotate for the SOHO mission text
    glPushMatrix();
    glTranslatef(-0.15f, 0.35f, 0.0f); // Adjust position
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Adjust rotation
    glScalef(0.01f, 0.01f, 0.01f); // Adjust text size

    // Render the SOHO mission text on the side of the ISS
    glColor3f(1.0, 1.0, 1.0); // Set text color (e.g., white)
    renderText("Solar and Heliospheric Observatory (SOHO) Mission");

    glPopMatrix();

    glPopMatrix();


}

void drawISS() {
    glPushMatrix();

    // Translate to the current position of the ISS along the x-axis
    glTranslatef(issXPosition, 2.8f, 0.0f);

    // Update the ISS position for animation (move from right to left)
    if (startAnimation) {
        issXPosition -= 0.002f; // Adjust the speed as needed
    }

    // Main body of ISS
    glColor3f(0.8f, 0.8f, 0.8f); // Gray color
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.1f);
    glVertex2f(0.2f, -0.1f);
    glVertex2f(0.2f, 0.1f);
    glVertex2f(-0.2f, 0.1f);
    glEnd();

    // Solar panels
    glColor3f(0.6f, 0.6f, 0.6f); // Dark gray color for solar panels
    glBegin(GL_QUADS);
    glVertex2f(-0.22f, -0.04f);
    glVertex2f(0.22f, -0.04f);
    glVertex2f(0.22f, 0.04f);
    glVertex2f(-0.22f, 0.04f);

    glVertex2f(-0.04f, -0.14f);
    glVertex2f(0.04f, -0.14f);
    glVertex2f(0.04f, 0.14f);
    glVertex2f(-0.04f, 0.14f);

    glVertex2f(-0.04f, -0.14f);
    glVertex2f(-0.14f, -0.14f);
    glVertex2f(-0.14f, 0.14f);
    glVertex2f(-0.04f, 0.14f);

    glVertex2f(0.04f, -0.14f);
    glVertex2f(0.14f, -0.14f);
    glVertex2f(0.14f, 0.14f);
    glVertex2f(0.04f, 0.14f);
    glEnd();

    glPopMatrix();
}

void drawBlackHole() {
    glPushMatrix();
    glTranslatef(4.0f, -2.1f, 0.0f);

    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the black hole
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(0.0f, 0.0f); // Center of the black hole

    // Draw a series of decreasing circles to represent the event horizon
    int numVertices = 50;
    float innerRadius = 0.05f; // Inner radius of the black hole
    float outerRadius = 0.1f; // Outer radius of the black hole
    for (int i = 0; i <= numVertices; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numVertices);
        float x = innerRadius * cos(angle);
        float y = innerRadius * sin(angle);
        glVertex2f(x, y);
    }

    glEnd();

    // Draw three white rings around the black hole
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glLineWidth(0.6f); // Line width for rings
    glBegin(GL_LINE_LOOP);

    // Outer ring
    for (int i = 0; i <= numVertices; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numVertices);
        float x = outerRadius * cos(angle);
        float y = outerRadius * sin(angle);
        glVertex2f(x, y);
    }

    // Middle ring
    outerRadius += 0.02f; // Adjust the radius for the middle ring
    for (int i = 0; i <= numVertices; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numVertices);
        float x = outerRadius * cos(angle);
        float y = outerRadius * sin(angle);
        glVertex2f(x, y);
    }

    // Inner ring
    outerRadius += 0.02f; // Adjust the radius for the inner ring
    for (int i = 0; i <= numVertices; ++i) {
        float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(numVertices);
        float x = outerRadius * cos(angle);
        float y = outerRadius * sin(angle);
        glVertex2f(x, y);
    }

    glEnd();

    glPopMatrix();
}

void drawEarth() {
    glPushMatrix();

    glRotatef(earthOrbitAngle, 0.0f, 1.0f, 0.0f);
    glTranslatef(earthOrbitRadius, 0.0f, -10.0f); // Earth's distance from the Sun
    glRotatef(earthOrbitAngle, 0.0f, 1.0f, 0.0f); // Rotate Earth around the Sun
    glScalef(0.3f, 0.3f, 0.3f); // Scale Earth


    // Draw the Earth's checkerboard-style surface

    glBegin(GL_TRIANGLES);

    // Draw green surface regions
    glColor3f(0.0f, 0.6f, 0.0f); // Green color
    for (const auto& polygon : landPolygons) {
        for (const auto& vertexIndex : polygon) {
            glVertex3fv(earthVertices[vertexIndex].data());
        }
    }

    // Draw blue surface regions
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    for (const auto& polygon : waterPolygons) {
        for (const auto& vertexIndex : polygon) {
            glVertex3fv(earthVertices[vertexIndex].data());
        }
    }

    glEnd();
    glPopMatrix();
}

// In the drawJupiter function, use the defined colors for bands
void drawJupiter() {
    glPushMatrix();
    glRotatef(sunOrbitAngle, 0.0f, 1.0f, 0.0f);
    glTranslatef(jupiterOrbitRadius, 0.0f, -20.0f); // Jupiter's distance from the Sun
    glRotatef(sunOrbitAngle, 0.0f, 1.0f, 0.0f); // Rotate Jupiter around the Sun
    glScalef(0.8f, 0.8f, 0.8f); // Scale Jupiter

    // Draw Jupiter as a sphere with color bands

    glPushMatrix();
    glTranslatef(5.0f, 0.0f, -5.0f); // Position Jupiter in the scene
    glRotatef(sunOrbitAngle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
    for (int i = 0; i < numRings; ++i) {
        for (int j = 0; j < numSegments; ++j) {
            int index0 = i * (numSegments + 1) + j;
            int index1 = index0 + 1;
            int index2 = (i + 1) * (numSegments + 1) + j;
            int index3 = index2 + 1;

            // Alternate between color bands for each row of quads
            int bandIndex = (i / (numRings / jupiterColors.size())) % jupiterColors.size();
            glColor3fv(jupiterColors[bandIndex].data());

            glVertex3fv(jupiterVertices[index0].data());
            glVertex3fv(jupiterVertices[index1].data());
            glVertex3fv(jupiterVertices[index3].data());
            glVertex3fv(jupiterVertices[index2].data());
        }
    }
    glEnd();

    glPopMatrix();


    glPopMatrix();
}

void drawVega() {
    // Draw Vega as a point marker
    glPushMatrix();
    glTranslatef(-5.0f, 4.5f, -5.0f); // Position Vega in the scene

    // Set the color and size for Vega's point marker
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(5.0f); // Adjust the point size as needed

    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f); // Vega's position (center of the scene)
    glEnd();

    glPopMatrix();
}

void drawStars() {
    // Draw stars as small points

    glPointSize(1.0f); // Set point size for stars

    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f); // White color for stars

    for (int i = 0; i < numStars; ++i) {
        glVertex3fv(stars[i]);
    }

    glEnd();
}

void drawSun() {
    // Draw the Sun as a glowing sphere

    // Set Sun's position
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f); // Move the Sun away from the camera

    // Apply the stored Sun rotation angle; the stylized sphere construction
    // does not make this rotation visibly meaningful
    glRotatef(sunAngle, 0.0f, 1.0f, 0.0f);

    // Define multi-colors for the Sun's surface
    GLfloat sunColors[6][3] = {
        {1.0f, 0.5f, 0.0f}, // Orange
        {1.0f, 0.8f, 0.0f}, // Yellow
        {1.0f, 0.5f, 0.0f}, // Orange
        {1.0f, 0.8f, 0.0f}, // Yellow
        {1.0f, 0.5f, 0.0f}, // Orange
        {1.0f, 0.8f, 0.0f}  // Yellow
    };

    int currentColor = 0; // Index for selecting colors

    // Draw the Sun with multi-colors
    for (int i = 0; i < 360; i += 60) {
        glColor3fv(sunColors[currentColor]);
        glRotatef(i, 0.0f, 1.0f, 0.0f); // Rotate to the next color segment
        glutSolidSphere(1.0, 50, 50); // Sphere segment
        currentColor = (currentColor + 1) % 6; // Cycle through colors
    }

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the camera position and orientation
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Draw stars
    drawStars();

    // Draw Vega Star
    drawVega();

    // Draw ISS in 3D
    drawISSThree();

    // Draw The ISS
    drawISS();

    // Draw BlackHole
    drawBlackHole();

    // Draw the Sun
    drawSun();

    // Draw Earth
    drawEarth();

    // Draw Jupiter
    drawJupiter();

    glutSwapBuffers();

    // Update the Earth's orbit angle for the next frame
    if (startAnimation) {
        earthOrbitAngle += 0.5f; // Adjust the orbit speed as needed
    }
    if (earthOrbitAngle >= 360.0f) {
        earthOrbitAngle -= 360.0f;
    }
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, (double)width / (double)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    // Update the stored Sun rotation angle
    if (startAnimation) {
        sunAngle += 0.5f;
    }

    // Update the angle used for the planetary orbit animation
    if (startAnimation) {
        sunOrbitAngle += 0.2f;
    }

    // Update Earth's orbit angle to make it orbit slower
    if (startAnimation) {
        earthOrbitAngle += 0.005f; // Adjust the orbit speed as needed (slower)
    }

    // Wrap angles to prevent overflow
    if (sunAngle >= 360.0f) {
        sunAngle -= 360.0f;
    }
    if (sunOrbitAngle >= 360.0f) {
        sunOrbitAngle -= 360.0f;
    }
    if (earthOrbitAngle >= 360.0f) {
        earthOrbitAngle -= 360.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0); // Update every 60 frames per second
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Solar System OpenGL Simulation");

    init();

    // Register the keyboard callback
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Start the timer for animation
    glutTimerFunc(0, timer, 0);

    glutMainLoop();

    return 0;
}
