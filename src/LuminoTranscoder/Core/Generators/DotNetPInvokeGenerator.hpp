#pragma once
#include "Generator.hpp"

class DotNetPInvokeGenerator
	: public Generator
{
public:
	void generate();

private:
    ln::String generateEnums();
    ln::String generateStructs();

    ln::String makeDocumentComment(DocumentInfo* doc) const;
};
