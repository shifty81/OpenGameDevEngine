# Game Assets

This directory is for game assets used in the engine and examples.

## Asset Types

### Textures (`textures/`)
- Image files: PNG, JPG, TGA
- Texture atlases
- UI sprites

### Models (`models/`)
- 3D model files: OBJ, FBX, GLTF
- Animation files
- Material definitions

### Audio (`audio/`)
- Sound effects: WAV, OGG
- Music: MP3, OGG
- Audio metadata

### Shaders (`shaders/`)
- Vertex shaders
- Fragment shaders
- Compute shaders

## Asset Pipeline

Assets can be processed using the asset conversion tool:
```bash
./bin/tools/AssetConverter input_file output_file
```

## Asset Guidelines

- Use version control for source assets
- Keep binary assets optimized for size
- Document asset sources and licenses
- Use consistent naming conventions

## License

Ensure all assets used are properly licensed for your project.
For example projects, use only:
- Public domain assets
- CC0 licensed assets
- Assets you have created yourself
