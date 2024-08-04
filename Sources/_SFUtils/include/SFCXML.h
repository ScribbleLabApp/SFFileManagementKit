//===-- include/SFCXML.h - XML Functions - libxml2 ----------------*- C -*-===//
//                                                                            //
// This source file is part of the Scribble Foundation open source project    //
//                                                                            //
// Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors     //
// Licensed under Apache License v2.0 with Runtime Library Exception          //
//                                                                            //
// You may not use this file except in compliance with the License.           //
// You may obtain a copy of the License at                                    //
//                                                                            //
//      http://www.apache.org/licenses/LICENSE-2.0                            //
//                                                                            //
// Unless required by applicable law or agreed to in writing, software        //
// distributed under the License is distributed on an "AS IS" BASIS,          //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   //
// See the License for the specific language governing permissions and        //
// limitations under the License.                                             //
//                                                                            //
//===----------------------------------------------------------------------===//
///
/// \file
///
//===----------------------------------------------------------------------===//

#ifndef SFCXML_h
#define SFCXML_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlwriter.h>

#define XML_ERR_PRASE -30

/**
 * Parses an XML file and prints the content of each 'item' element.
 *
 * @param filename The path to the XML file to be parsed.
 *
 * This function initializes the XML parser, reads the specified XML file, and then iterates through each 'item' element.
 * For each 'item' element, it prints the name of each child element and its corresponding content.
 * If the XML file cannot be parsed, an error message is printed to stderr.
 *
 * @return void
 */
void parseXML(const char* filename) {
    xmlDoc *doc = NULL;                   // TODO: NULL ~> nullptr
    xmlNode *root_element = NULL;
    
    xmlInitParser();
    
    doc = xmlReadFile(filename, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse XML file: %s\n", filename);
        return;
    }

    root_element = xmlDocGetRootElement(doc);

    for (xmlNode *node = root_element->children; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, (const xmlChar *)"item") == 0) {
            xmlNode *child = node->children;
            while (child) {
                if (child->type == XML_ELEMENT_NODE) {
                    printf("Element: %s\n", child->name);
                    xmlNode *textNode = child->children;
                    if (textNode && textNode->type == XML_TEXT_NODE) {
                        printf("Content: %s\n", xmlNodeGetContent(textNode));
                    }
                }
                child = child->next;
            }
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
}

/**
 * Initializes and starts a new XML document for writing.
 *
 * @param filename The name of the file where the XML document will be written.
 *
 * This function creates a new XML writer for the specified file and starts the XML document.
 * If the XML writer cannot be created, an error message is printed to stderr and the function returns NULL.
 *
 * @return A pointer to the XML writer if successful, or NULL if an error occurred.
 */
xmlTextWriterPtr startXMLDocument(const char *filename) {
    xmlTextWriterPtr writer = xmlNewTextWriterFilename(filename, 0);
    if (writer == NULL) {
        fprintf(stderr, "Error creating the xml writer\n");
        return NULL;
    }
    xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
    return writer;
}

/**
 * Writes an XML element with the given name and content to the specified writer.
 *
 * @param writer The XML writer to which the element will be written.
 * @param elementName The name of the XML element to be written.
 * @param textContent The content of the XML element. If this parameter is NULL, no text content will be written.
 *
 * This function starts a new XML element with the given name, writes the specified text content (if not NULL),
 * and then ends the XML element.
 *
 * @return void
 */
void writeElement(xmlTextWriterPtr writer, const char *elementName, const char *textContent) {
    xmlTextWriterStartElement(writer, (const xmlChar *)elementName);
    if (textContent != NULL) {
        xmlTextWriterWriteString(writer, (const xmlChar *)textContent);
    }
    xmlTextWriterEndElement(writer);
}


/**
 * Writes an XML 'item' element with the given attributes and child elements to the specified writer.
 *
 * @param writer The XML writer to which the 'item' element will be written.
 * @param id The unique identifier for the 'item'.
 * @param type The type of the 'item'.
 * @param x The x-coordinate of the 'item' position.
 * @param y The y-coordinate of the 'item' position.
 * @param width The width of the 'item'.
 * @param height The height of the 'item'.
 * @param textFile The name of the text file associated with the 'item'.
 *
 * This function starts a new 'item' element with the given attributes, writes child elements for position, size, and text,
 * and then ends the 'item' element. The position and size elements contain child elements for x, y, width, and height, respectively.
 *
 * @return void
 */
void writeItem(xmlTextWriterPtr writer, const char *id, const char *type, int x, int y, int width, int height, const char *textFile) {
    char buffer[20];
    
    xmlTextWriterStartElement(writer, (const xmlChar *)"item");
    
    writeElement(writer, "id", id);
    writeElement(writer, "type", type);
    
    xmlTextWriterStartElement(writer, (const xmlChar *)"position");
    sprintf(buffer, "%d", x);
    writeElement(writer, "x", buffer);
    
    sprintf(buffer, "%d", y);
    writeElement(writer, "y", buffer);
    xmlTextWriterEndElement(writer);
    
    xmlTextWriterStartElement(writer, (const xmlChar *)"size");
    sprintf(buffer, "%d", width);
    writeElement(writer, "width", buffer);
    
    sprintf(buffer, "%d", height);
    writeElement(writer, "height", buffer);
    xmlTextWriterEndElement(writer);
    
    writeElement(writer, "text", textFile);
    
    xmlTextWriterEndElement(writer);
}

/**
 * Closes the XML document and frees the associated resources.
 *
 * @param writer The XML writer for the document to be closed.
 *
 * This function ends the XML document and frees the resources associated with the specified XML writer.
 * After calling this function, the writer should not be used for further XML writing operations.
 *
 * @return void
 */
void endXMLDocument(xmlTextWriterPtr writer) {
    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
}

#ifdef __cplusplus
}
#endif

#endif /* SFCXML_h */
