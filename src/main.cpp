#include <SFML/Graphics.hpp>
#include"Parser.h"
#include"Renderer.h"

int main()
{
    Parser parser;
    // Add your SVG file address.
    const string filePath = "D:\\TLHT\\OOP\\Project\\SVGReaderProject\\samples\\testSample.svg";
   
    if (!parser.loadFile(filePath)) {
        cout << "Cannot open file or read file" << endl;
        return 0;
    }
    Renderer render(parser.getShape());
    render.run();



    return 0;
}