```c++
void glmTranslationExample()
{
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // define and initalize an identity matrix (or else it will be a null matrix)
    glm::mat4 translationMatrix = glm::mat4(1.0f);
    // create a transformation matrix, pass the identity matrix and the vector we want to multiply it with
    translationMatrix = glm::translate(translationMatrix, glm::vec3(1.0f, 1.0f, 0.0f));
    // multiply our vector by the transformation matrix
    vec = translationMatrix * vec;
    std::cout << vec.x << vec.y << vec.z << vec.w << std::endl;
    // vec: 1, 0, 0, 1 becomes 2, 1, 0, 1
    // after being multiplied by a translationMatrix where Tx, Ty, Tz is 1, 1, 0
}
```