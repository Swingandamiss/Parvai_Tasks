#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Forward ultrasonic sensor pins
#define TRIG_FRONT 18
#define ECHO_FRONT 19

// Downward ultrasonic sensor pins
#define TRIG_DOWN 10
#define ECHO_DOWN 2

// LED/Buzzer pin
#define ALERT_PIN 3

// Variables for distances
float forward_cm;
float downward_cm;

// Variables for accelerometer values
float ax, ay, az;
float magnitude;

// Baseline distance for downward sensor
float baseline_downward = 50.0;

// Variables for millis timing
unsigned long ultrasonicTimer = 0;
unsigned long mpuTimer = 0;
unsigned long logTimer = 0;

// Variables for alert timing
unsigned long alertStartTime = 0;
bool alertActive = false;

// Variables for fall detection
bool freeFall = false;
unsigned long freeFallStart = 0;

// Function to read ultrasonic sensor
float readDistance(int trigPin, int echoPin)
{
    long duration;
    float distance;
    // Send trigger pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Read echo pulse
    duration = pulseIn(echoPin, HIGH);
    // Convert time to distance
    distance = duration * 0.034 / 2;
    return distance;
}

void setup()
{
    Serial.begin(115200);
    // Ultrasonic sensor pin setup
    pinMode(TRIG_FRONT, OUTPUT);
    pinMode(ECHO_FRONT, INPUT);
    pinMode(TRIG_DOWN, OUTPUT);
    pinMode(ECHO_DOWN, INPUT);

    // Alert output pin
    pinMode(ALERT_PIN, OUTPUT);

    // Start I2C communication
    Wire.begin();

    // Initialize MPU6050
    mpu.initialize();
    Serial.println("System Starting");
}
void loop()
{
    unsigned long currentTime = millis();
    // Read ultrasonic sensors every 100ms
    if(currentTime - ultrasonicTimer >= 100)
    {
        ultrasonicTimer = currentTime;
        // Read front ultrasonic sensor
        forward_cm = readDistance(TRIG_FRONT, ECHO_FRONT);
        // Read downward ultrasonic sensor
        downward_cm = readDistance(TRIG_DOWN, ECHO_DOWN);
        // Detect forward obstacle
        if(forward_cm < 80)
        {
            digitalWrite(ALERT_PIN, HIGH);
            alertActive = true;
            // Short buzz duration
            alertStartTime = currentTime;
        }
        // Detect downward drop
        if((downward_cm - baseline_downward) > 20)
        {
            digitalWrite(ALERT_PIN, HIGH);
            alertActive = true;
            // Long buzz duration
            alertStartTime = currentTime;
        }
    }
    // Read MPU6050 every 10ms
    if(currentTime - mpuTimer >= 10)
    {
        mpuTimer = currentTime;
        int16_t raw_ax, raw_ay, raw_az;
        // Read accelerometer values
        mpu.getAcceleration(&raw_ax, &raw_ay, &raw_az);
        // Convert raw values to g values
        ax = raw_ax / 16384.0;
        ay = raw_ay / 16384.0;
        az = raw_az / 16384.0;
        // 16384 counts represents 1g in ±2g mode
        // Calculate total acceleration magnitude
        magnitude = sqrt(ax * ax + ay * ay + az * az);
        // Detect free fall
        if(magnitude < 0.5)
        {
            // Start free fall timer
            if(!freeFall)
            {
                freeFall = true;
                freeFallStart = currentTime;
            }
        }
        // Detect impact after free fall
        if(freeFall)
        {
            // Check if free fall lasted for 100ms
            if(currentTime - freeFallStart >= 100)
            {
                // Detect impact
                if(magnitude > 3.5)
                {
                    digitalWrite(ALERT_PIN, HIGH);
                    alertActive = true;
                    alertStartTime = currentTime;
                }
            }
        }
        // Reset free fall condition
        if(magnitude > 0.5)
        {
            freeFall = false;
        }
    }
    // Turn OFF alert without using delay()
    if(alertActive)
    {
        // Turn off after 500ms
        if(currentTime - alertStartTime >= 500)
        {
            digitalWrite(ALERT_PIN, LOW);
            alertActive = false;
        }
    }
    // Serial logging every 500ms
    if(currentTime - logTimer >= 500)
    {
        logTimer = currentTime;
        Serial.print("Time: ");
        Serial.print(currentTime);
        Serial.print(" ms  ");
        Serial.print("Forward: ");
        Serial.print(forward_cm);
        Serial.print(" cm  ");
        Serial.print("Downward: ");
        Serial.print(downward_cm);
        Serial.print(" cm  ");
        Serial.print("Magnitude: ");
        Serial.print(magnitude);
        Serial.print("  Alert State: ");
        if(magnitude > 3.5)
        {
            Serial.println("FALL DETECTED");
        }
        else if(forward_cm < 80)
        {
            Serial.println("OBSTACLE DETECTED");
        }
        else if((downward_cm - baseline_downward) > 20)
        {
            Serial.println("DROP DETECTED");
        }
        else
        {
            Serial.println("NORMAL");
        }
    }
}