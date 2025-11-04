# Camera System Usage Guide

The OpenGameDevEngine camera system provides perspective and orthographic projections for 3D rendering.

## Quick Start

### Creating a Camera

```cpp
#include "ogde/graphics/Camera.h"

// Create a camera with default perspective projection
ogde::graphics::Camera camera;

// Configure perspective projection
camera.setPerspective(45.0f,         // Field of view (degrees)
                     16.0f / 9.0f,   // Aspect ratio
                     0.1f,           // Near plane
                     100.0f);        // Far plane

// Position the camera
camera.lookAt(0.0f, 2.0f, -5.0f,    // Camera position
             0.0f, 0.0f, 0.0f,      // Look at target
             0.0f, 1.0f, 0.0f);     // Up vector

// Update matrices
camera.update();
```

### Using Camera Matrices in Shaders

The camera provides view and projection matrices that can be used in your shaders:

```cpp
// Get the view-projection matrix
const float* vpMatrix = camera.getViewProjectionMatrix();

// Upload to constant buffer
ConstantBuffer cb;
memcpy(cb.viewProjectionMatrix, vpMatrix, sizeof(float) * 16);

// Update DirectX constant buffer
D3D11_MAPPED_SUBRESOURCE mappedResource;
deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
memcpy(mappedResource.pData, &cb, sizeof(ConstantBuffer));
deviceContext->Unmap(constantBuffer, 0);
```

### HLSL Shader Example

```hlsl
cbuffer ConstantBuffer : register(b0) {
    matrix viewProjectionMatrix;
    matrix modelMatrix;
};

struct VSInput {
    float3 position : POSITION;
    float4 color : COLOR;
};

struct PSInput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput VSMain(VSInput input) {
    PSInput output;
    
    // Transform vertex position
    float4 worldPos = mul(float4(input.position, 1.0f), modelMatrix);
    output.position = mul(worldPos, viewProjectionMatrix);
    output.color = input.color;
    
    return output;
}
```

## Camera Types

### Perspective Projection

Best for 3D scenes with depth perception:

```cpp
camera.setPerspective(
    45.0f,          // FOV in degrees (typically 45-90)
    aspectRatio,    // Width / Height
    0.1f,          // Near plane (should be > 0)
    1000.0f        // Far plane
);
```

### Orthographic Projection

Best for 2D games and UI:

```cpp
camera.setOrthographic(
    800.0f,        // Width
    600.0f,        // Height
    -1.0f,         // Near plane
    1.0f           // Far plane
);
```

## Camera Positioning

### Using Look-At

The easiest way to position a camera:

```cpp
camera.lookAt(
    eyeX, eyeY, eyeZ,       // Camera position
    targetX, targetY, targetZ,  // Where to look
    upX, upY, upZ           // Up direction (usually 0,1,0)
);
```

### Using Position and Rotation

For more control:

```cpp
camera.setPosition(x, y, z);
camera.setRotation(pitch, yaw, roll);  // In degrees
```

## Getting Camera Information

### Position

```cpp
float x, y, z;
camera.getPosition(x, y, z);
```

### Direction Vectors

```cpp
float fx, fy, fz, rx, ry, rz, ux, uy, uz;
camera.getForward(fx, fy, fz);  // Forward direction
camera.getRight(rx, ry, rz);    // Right direction
camera.getUp(ux, uy, uz);       // Up direction
```

### Matrices

```cpp
const float* viewMatrix = camera.getViewMatrix();
const float* projectionMatrix = camera.getProjectionMatrix();
const float* vpMatrix = camera.getViewProjectionMatrix();
```

## Complete Example

See `examples/3d-demo/camera_demo.cpp` for a complete working example that:
- Creates a perspective camera
- Positions it with look-at
- Renders a rotating 3D cube
- Updates the view-projection matrix each frame

## Camera Conventions

The camera system uses DirectX's left-handed coordinate system:
- **X-axis**: Points right
- **Y-axis**: Points up
- **Z-axis**: Points forward (into the scene)

## Matrix Format

All matrices are stored in **column-major** format as 16 floats, compatible with DirectX 11 constant buffers.

## Performance Tips

1. **Only update when needed**: Call `camera.update()` only when position or projection changes
2. **Reuse matrices**: Store the view-projection matrix if it doesn't change every frame
3. **Batch updates**: Update multiple objects with the same camera matrices in one draw call

## Common Issues

### Upside-down rendering
- Check your up vector in `lookAt()` - it should be (0, 1, 0) for Y-up
- Verify your projection matrix matches your coordinate system

### Nothing visible
- Ensure near plane > 0 for perspective projection
- Check that objects are within the near/far plane range
- Verify camera is positioned correctly relative to objects

### Objects appear stretched
- Check aspect ratio matches your window dimensions
- Recalculate aspect ratio when window resizes:
  ```cpp
  camera.setPerspective(45.0f, width / height, 0.1f, 100.0f);
  camera.update();
  ```
