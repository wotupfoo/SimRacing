# Creating a Markdown file with the answer.

md_content = """
# Calculation of Required Power for Rotational Motion

To calculate the required power for a DC motor to rotate a 200 kg mass by 20 degrees in 250 ms with zero angular velocity at the end, we follow these steps:

## Given Data
- **Mass (m)**: 200 kg
- **Distance of Center of Mass from Pivot (r)**: 0.25 m
- **Distance of Applied Force from Pivot (d)**: 0.5 m
- **Rotation Angle (θ)**: 20° (converted to radians below)
- **Time (t)**: 0.25 s

## Step-by-Step Solution

### Step 1: Convert Angle to Radians
To work with the angle in radians:
\(
\\theta = 20^\circ \\times \\frac{\\pi}{180} = 0.349 \\, \\text{radians}
\)

### Step 2: Calculate Half-Angle and Half-Time for Symmetric Motion
Since we want the angular velocity to be zero at the end, the motion is symmetric (acceleration to midpoint and deceleration thereafter):
\(
\\theta_{\\text{half}} = \\frac{0.349}{2} = 0.175 \\, \\text{radians}
\)
\(
t_{\\text{half}} = \\frac{0.25}{2} = 0.125 \\, \\text{s}
\)

### Step 3: Calculate the Required Angular Acceleration (α) for Half Motion
Using the formula \\( \\theta = 0.5 \\alpha t^2 \\):
\(
\\alpha = \\frac{2 \\theta_{\\text{half}}}{t_{\\text{half}}^2} = \\frac{2 \\times 0.175}{(0.125)^2} = 22.34 \\, \\text{rad/s}^2
\)

### Step 4: Calculate the Moment of Inertia (I)
For a point mass located a distance \\( r \\) from the pivot:
\(
I = m \\cdot r^2 = 200 \\times (0.25)^2 = 12.5 \\, \\text{kg} \\cdot \\text{m}^2
\)

### Step 5: Calculate the Required Torque (τ)
Using the relationship between torque and angular acceleration:
\(
\\tau = I \\cdot \\alpha = 12.5 \\times 22.34 = 279.25 \\, \\text{Nm}
\)

### Step 6: Calculate Peak Angular Velocity (ω) at Halfway Point
At the peak (midway), the angular velocity is:
\(
\\omega_{\\text{peak}} = \\alpha \\cdot t_{\\text{half}} = 22.34 \\times 0.125 = 2.79 \\, \\text{rad/s}
\)

### Step 7: Calculate Power Required at Peak Torque and Peak Angular Velocity
\(
\\text{Power} = \\tau \\cdot \\omega_{\\text{peak}} = 279.25 \\times 2.79 = 779.82 \\, \\text{W}
\)

## Answer
To rotate the mass by 20 degrees in 250 ms while ending with zero angular velocity, the motor would need to supply approximately **779.82 watts** at peak power.
"""

# Write content to a markdown file
with open("rotational_power_calculation.md", "w") as file:
    file.write(md_content)

"rotational_power_calculation.md"
