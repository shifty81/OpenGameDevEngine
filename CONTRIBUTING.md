# Contributing to OpenGameDevEngine

Thank you for your interest in contributing to OpenGameDevEngine!

## How to Contribute

1. Fork the repository
2. Create a new branch for your feature or bugfix
3. Make your changes
4. Write or update tests as needed
5. Ensure all tests pass
6. Submit a pull request

## Code Style

- Use C++17 features where appropriate
- Follow the existing code style in the project
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and concise

## Project Structure

Please maintain the existing directory structure:
- `src/` - Implementation files (.cpp)
- `include/ogde/` - Public header files (.h, .hpp)
- `tests/` - Unit tests
- `examples/` - Example projects
- `docs/` - Documentation

## Adding New Features

When adding a new feature:

1. Place implementation in the appropriate module under `src/`
2. Add public API headers to `include/ogde/`
3. Update the corresponding CMakeLists.txt
4. Add tests for the new functionality
5. Update documentation

## Testing

Run tests before submitting:
```bash
cd build
ctest
```

## Pull Request Process

1. Update the README.md with details of changes if applicable
2. Update documentation for any API changes
3. The PR will be merged once approved by maintainers

## Code of Conduct

- Be respectful and constructive
- Welcome newcomers and help them learn
- Focus on what is best for the community
- Show empathy towards other community members

## Questions?

Feel free to open an issue for questions or discussions.

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
