#ifndef SIMPLE_HTML_WRITER_H
#define SIMPLE_HTML_WRITER_H
//----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>


/**
//----------------------------------------------------------------------------
void    Test(std::string output_filename)
{
    // Line break, for convenience:
    shared_ptr<Break>   line_break(new Break());

    // Create the document:
    Document    doc;

    // Append head and title:
    auto    head = doc.AppendChild(make_shared<Head>());
    head->AppendChild(make_shared<Title>("Title"));
    head->AppendChild(make_shared<CSSResourceLink>("stylesheet", "http://www.w3schools.com/lib/w3.css"));

    // Append body:
    auto    body = doc.AppendChild(make_shared<Body>());

    // Add elements to the body...
    // Heading with id:
    body->AppendChild(make_shared<Heading>("Heading", 1))
            ->AppendId("lige_her");

    // Paragraph:
    auto paragraph = body->AppendChild(make_shared<Paragraph>("Hej med dig, "));
    paragraph->AppendChild(make_shared<Link>("https://en.wikipedia.org/wiki/Ole_R%C3%B8mer", "Ole Roemer"));
    paragraph->AppendChild(make_shared<Text>(", min gode ven!"));
    paragraph->AppendChild(line_break);
    paragraph->AppendChild(make_shared<Text>("Har du set noget til Tycho?"));

    body->AppendChild(line_break);

    // Text link:
    body->AppendChild(make_shared<Link>("http://www.lmwindpower.com", "lm windpower"));
    body->AppendChild(line_break);

    // Image:
    body->AppendChild(make_shared<Image>("plot.png", "hej", 256, 256));
    body->AppendChild(line_break);

    // Image link with reference to internal id:
    body->AppendChild(make_shared<Link>("#lige_her", ""))
            ->AppendChild(make_shared<Image>("plot.png", "hej", 64, 64));
    body->AppendChild(line_break);

    // List, can be Ordered or Unordered:
    auto list = body->AppendChild(make_shared<UnorderedList>());
    list->AppendChild(make_shared<ListItem>("item 1"));
    list->AppendChild(make_shared<ListItem>("item 2"));
    list->AppendChild(make_shared<ListItem>("item 3"));

    // Table:
    auto    table = body->AppendChild(make_shared<Table>("Hallo du der!!!"));
    auto    table_header = table->AppendChild(make_shared<TableRow>());
    table_header->AppendChild(make_shared<TableHeaderElement>("A"));
    table_header->AppendChild(make_shared<TableHeaderElement>("B"));
    table_header->AppendChild(make_shared<TableHeaderElement>("C"));
    {
        auto    table_row = table->AppendChild(make_shared<TableRow>());
        table_row->AppendChild(make_shared<TableElement>("a"));
        table_row->AppendChild(make_shared<TableElement>("b"));
        table_row->AppendChild(make_shared<TableElement>("c"));
    }
    {
        auto    table_row = table->AppendChild(make_shared<TableRow>());
        table_row->AppendChild(make_shared<TableElement>("1"));
        table_row->AppendChild(make_shared<TableElement>("2"));
        table_row->AppendChild(make_shared<TableElement>("3"));
    }

    // Output document to stream:
    ofstream    file("../ole_roemer.html");
    file << doc;
}
 */

namespace simple_html
{
template<typename T, typename... Args>
std::shared_ptr<T>  Get(Args&&... args)
/// From: http://eli.thegreenplace.net/2014/variadic-templates-in-c/
{
//    return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
    return std::make_shared<T>(std::forward<Args>(args)...);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 * @brief The AttributeBase class is a base class for attibutes.
 */
class   AttributeBase
{
public:
    std::string name{""};
    virtual std::string Get(){return "";}

    AttributeBase(std::string name)
        : name(name)
    {
#ifdef __DEBUG
        std::cout << "Constructing AttributeBase" << std::endl;
#endif
    }
    virtual ~AttributeBase()
    {
#ifdef __DEBUG
        std::cout << "Destructing AttributeBase" << std::endl;
#endif
    }
};
//----------------------------------------------------------------------------
/**
 * @brief The Attribute class handles a generic attribute like <..... attribute="value">.
 */
class   Attribute : public AttributeBase
{
    std::string value{};
public:
    Attribute(std::string name, std::string value)
        : AttributeBase(name),
          value(value)
    {
#ifdef __DEBUG
        std::cout << "Constructing Attribute" << std::endl;
#endif
    }
    Attribute(std::string name, int value_int)
        : AttributeBase(name),
          value(std::to_string(value_int))
    {
#ifdef __DEBUG
        std::cout << "Constructing Attribute" << std::endl;
#endif
    }
    Attribute(std::string name, double value_double)
        : AttributeBase(name),
          value(std::to_string(value_double))
    {
#ifdef __DEBUG
        std::cout << "Constructing Attribute" << std::endl;
#endif
    }
    virtual ~Attribute()
    {
#ifdef __DEBUG
        std::cout << "Destructing Attribute" << std::endl;
#endif
    }

    virtual std::string Get() override {return name + "=" + "\"" + value + "\"";}
};

//----------------------------------------------------------------------------
/**
 * @brief The IdAttribute class handles a generic attribute like <..... id="value">.
 */
class   IdAttribute : public Attribute
{
    std::string value{};
public:
    IdAttribute(std::string identifier)
        : Attribute("id", identifier)
    {
#ifdef __DEBUG
        std::cout << "Constructing IdAttribute" << std::endl;
#endif
    }
    virtual ~IdAttribute()
    {
#ifdef __DEBUG
        std::cout << "Destructing IdAttribute" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The ClassAttribute class handles a generic attribute like <..... class="">.
 */
class   ClassAttribute : public Attribute
{
    std::string value{};
public:
    ClassAttribute(std::string identifier)
        : Attribute("class", identifier)
    {
#ifdef __DEBUG
        std::cout << "Constructing ClassAttribute" << std::endl;
#endif
    }
    virtual ~ClassAttribute()
    {
#ifdef __DEBUG
        std::cout << "Destructing ClassAttribute" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 * @brief The NodeBase class is a base class for nodes.
 */
class   NodeBase
{
protected:
    std::string name;
    std::string value;
    bool        _is_inline{false};

    const char  indent_char{'\t'};
    std::ostream&   StartTag(std::ostream &stream)
    {
        stream << "<" << name;

        for (auto &a : attributes)
        {
            stream << " " << a->Get();
        }

        stream << ">";

        return stream;
    }

    virtual std::ostream&   EndTag(std::ostream &stream)
    {
        stream << "</" << name << ">";

        return stream;
    }

    std::ostream&   WriteIdentation(std::ostream &stream, int indentation)
    {
        if (!this->is_inline())
        {
            stream << std::string(indentation, indent_char);
        }

        return stream;
    }

    std::vector<std::shared_ptr<NodeBase>>  children;
    std::vector<std::shared_ptr<AttributeBase>> attributes;
public:
    NodeBase(std::string name)
        : name(name),
          value("")
    {
#ifdef __DEBUG
        std::cout << "Constructing NodeBase" << std::endl;
#endif
    }
    NodeBase(std::string name, std::string value)
        : name(name),
          value(value)
    {
#ifdef __DEBUG
        std::cout << "Constructing NodeBase" << std::endl;
#endif
    }

    virtual ~NodeBase()
    {
#ifdef __DEBUG
        std::cout << "Destructing NodeBase" << std::endl;
#endif
    }

    std::shared_ptr<Attribute>    AppendAttribute(const std::shared_ptr<Attribute> &a)
    {
        attributes.push_back(a);
        return a;
    }

    std::shared_ptr<NodeBase>    AppendChild(const std::shared_ptr<NodeBase> &a)
    {
        children.push_back(a);
        return a;
    }

    std::shared_ptr<Attribute>    AppendId(std::string id)
    {
        return AppendAttribute(std::make_shared<IdAttribute>(id));
    }

    std::shared_ptr<Attribute>  AppendClass(std::string name)
    {
        return AppendAttribute(std::make_shared<ClassAttribute>(name));
    }

    virtual std::string Get(int indentation = 0)
    {
        std::stringstream   stream;

        WriteIdentation(stream, indentation);
        StartTag(stream);

        if (value.length() > 0)
        {
            stream << std::endl << std::string(indentation + 1, indent_char);
            stream << value;
        }

        for (auto &c : children)
        {
            if (!c->is_inline())
            {
                stream << std::endl;
            }
            stream << c->Get(indentation + 1);
//            stream << std::string(indentation, indent_char) << c->Get(indentation + 1);
        }

        stream << std::endl << std::string(indentation, indent_char);
        EndTag(stream);
//        stream << std::endl;

        return stream.str();
    }

    bool    is_inline() {return _is_inline;}

    friend  std::ostream& operator<<(std::ostream &stream, NodeBase &node);
};

inline  std::ostream& operator<<(std::ostream &stream, NodeBase &node)
{
    return stream << node.Get();
}

inline  std::shared_ptr<NodeBase>   GetNodeBase(std::string name)
{
    return std::make_shared<NodeBase>(name);
}

inline  std::shared_ptr<NodeBase>   GetNodeBase(std::string name, std::string value)
{
    return std::make_shared<NodeBase>(name, value);
}


//----------------------------------------------------------------------------
/**
 * @brief The Void class handles a void node (node without end tag) like <br> or <img>.
 */
class   Void : public NodeBase
{
    std::ostream&   EndTag(std::ostream &stream) override
    {
        return stream;
    }
public:
    Void(std::string name)
        : NodeBase(name)
    {
#ifdef __DEBUG
        std::cout << "Constructing Void" << std::endl;
#endif
    }
    virtual ~Void()
    {
#ifdef __DEBUG
        std::cout << "Destructing Void" << std::endl;
#endif
    }

    virtual std::string Get(int indentation = 0) override
    {
        std::stringstream   stream;

        WriteIdentation(stream, indentation);
        StartTag(stream);

        return stream.str();
    }
};

inline  std::shared_ptr<Void>   GetVoid(std::string name)
{
    return std::make_shared<Void>(name);
}

//----------------------------------------------------------------------------
/**
 * @brief The NodeLine class handles a one-line node.
 */
class   NodeLine : public NodeBase
{
public:
    NodeLine(std::string name)
        : NodeBase(name)
    {
#ifdef __DEBUG
        std::cout << "Constructing NodeLine" << std::endl;
#endif
    }
    NodeLine(std::string name, std::string value)
        : NodeBase(name, value)
    {
#ifdef __DEBUG
        std::cout << "Constructing NodeLine" << std::endl;
#endif
    }
    virtual ~NodeLine()
    {
#ifdef __DEBUG
        std::cout << "Destructing NodeLine" << std::endl;
#endif
    }

    virtual std::string Get(int indentation = 0) override
    {
        std::stringstream   stream;

        WriteIdentation(stream, indentation);
        StartTag(stream);

        if (value.length() > 0)
        {
            stream << value;
        }

        for (auto &c : children)
        {
            if (!c->is_inline())
            {
                stream << std::endl;
            }
            stream << c->Get(indentation + 1);
        }

        EndTag(stream);

        return stream.str();
    }
};

inline  std::shared_ptr<NodeLine>   GetNodeLine(std::string name)
{
    return std::make_shared<NodeLine>(name);
}

inline  std::shared_ptr<NodeLine>   GetNodeLine(std::string name, std::string value)
{
    return std::make_shared<NodeLine>(name, value);
}

//----------------------------------------------------------------------------
/**
 * @brief The NodeInline class handles a one-line node inserted inline.
 */
class   NodeInline : public NodeLine
{
public:
    NodeInline(std::string name)
        : NodeLine(name)
    {
        _is_inline = true;
#ifdef __DEBUG
        std::cout << "Constructing NodeInline" << std::endl;
#endif
    }
    NodeInline(std::string name, std::string value)
        : NodeLine(name, value)
    {
        _is_inline = true;
#ifdef __DEBUG
        std::cout << "Constructing NodeInline" << std::endl;
#endif
    }
    virtual ~NodeInline()
    {
#ifdef __DEBUG
        std::cout << "Destructing NodeInline" << std::endl;
#endif
    }

    virtual std::string Get(int indentation = 0) override
    {
        std::stringstream   stream;

        WriteIdentation(stream, indentation);
        StartTag(stream);

        if (value.length() > 0)
        {
            stream << value;
        }

        for (auto &c : children)
        {
            stream << c->Get(indentation + 1);
        }

        EndTag(stream);

        return stream.str();
    }
};

inline  std::shared_ptr<NodeInline>   GetNodeInline(std::string name)
{
    return std::make_shared<NodeInline>(name);
}

inline  std::shared_ptr<NodeInline>   GetNodeInline(std::string name, std::string value)
{
    return std::make_shared<NodeInline>(name, value);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 * @brief The Document class handles a document node, <html></html>.
 */
class   Document : public NodeBase
{
public:
    Document()
        : NodeBase("html")
    {
#ifdef __DEBUG
        std::cout << "Constructing Document" << std::endl;
#endif
    }

    virtual ~Document()
    {
#ifdef __DEBUG
        std::cout << "Destructing Document" << std::endl;
#endif
    }

    virtual std::string Get(int indentation = 0) override
    {
        std::stringstream   stream;
        stream << "<!DOCTYPE html>" << std::endl;
        stream << NodeBase::Get(indentation);

        return stream.str();
    }
};

inline  std::ostream& operator<<(std::ostream &stream, Document &node)
{
    return stream << node.Get();
}


//----------------------------------------------------------------------------
/**
 * @brief The Head class handles a head node, <head></head>.
 */
class   Head : public NodeBase
{
public:
    Head()
        : NodeBase("head")
    {
#ifdef __DEBUG
        std::cout << "Constructing Head" << std::endl;
#endif
    }

    virtual ~Head()
    {
#ifdef __DEBUG
        std::cout << "Destructing Head" << std::endl;
#endif
    }
};

inline  std::shared_ptr<Head>   GetHead()
{
    return std::make_shared<Head>();
}

//----------------------------------------------------------------------------
/**
 * @brief The Body class handles a body node, <body></body>.
 */
class   Body : public NodeBase
{
public:
    Body()
        : NodeBase("body")
    {
#ifdef __DEBUG
        std::cout << "Constructing Body" << std::endl;
#endif
    }

    virtual ~Body()
    {
#ifdef __DEBUG
        std::cout << "Destructing Body" << std::endl;
#endif
    }
};

inline  std::shared_ptr<Body>   GetNodeInline()
{
    return std::make_shared<Body>();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 * @brief The ResourceLink class handles a resource link, <link rel="relation">.
 */
class   ResourceLink : public Void
{
public:
    ResourceLink(std::string relation)
        : Void("link")
    {
        this->AppendAttribute(std::shared_ptr<Attribute>(new Attribute("rel", relation)));
#ifdef __DEBUG
        std::cout << "Constructing ResourceLink" << std::endl;
#endif
    }

    virtual ~ResourceLink()
    {
#ifdef __DEBUG
        std::cout << "Destructing ResourceLink" << std::endl;
#endif
    }
};

inline  std::shared_ptr<ResourceLink>   GetResourceLink(std::string relation)
{
    return std::make_shared<ResourceLink>(relation);
}

//----------------------------------------------------------------------------
/**
 * @brief The CSSResourceLink class handles a css resource link, <link rel="relation", href="url", type="text/css">.
 */
class   CSSResourceLink : public ResourceLink
{
public:
    CSSResourceLink(std::string relation, std::string url)
        : ResourceLink(relation)
    {
        this->AppendAttribute(std::shared_ptr<Attribute>(new Attribute("href", url)));
        this->AppendAttribute(std::shared_ptr<Attribute>(new Attribute("type", "text/css")));
#ifdef __DEBUG
        std::cout << "Constructing ResourceLink" << std::endl;
#endif
    }

    virtual ~CSSResourceLink()
    {
#ifdef __DEBUG
        std::cout << "Destructing ResourceLink" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The Link class handles a one-line link tag <a href="url">text</a>.
 */
class   Link : public NodeInline
{
public:
    Link(std::string url, std::string text)
        : NodeInline("a", text)
    {
        this->AppendAttribute(std::shared_ptr<Attribute>(new Attribute("href", url)));
#ifdef __DEBUG
        std::cout << "Constructing Link" << std::endl;
#endif
    }

    virtual ~Link()
    {
#ifdef __DEBUG
        std::cout << "Destructing Link" << std::endl;
#endif
    }
};

inline  std::shared_ptr<Link>   GetLink(std::string url, std::string text)
{
    return std::make_shared<Link>(url, text);
}

//----------------------------------------------------------------------------
/**
 * @brief The Image class handles an image tag, <img ...>.
 */
class   Image : public Void
{
public:
    //<img src="pic_mountain.jpg" alt="Mountain View" style="width:304px;height:228px;">
    Image(std::string url, std::string alt_text, int width, int height, bool old_style = false)
        : Void("img")
    {
        _is_inline = true;
#ifdef __DEBUG
        std::cout << "Constructing Image" << std::endl;
#endif

        this->AppendAttribute(std::shared_ptr<Attribute>(new Attribute("src", url)));
        this->AppendAttribute(std::shared_ptr<Attribute>(new Attribute("alt", alt_text)));

        if (!old_style)
        {
        std::stringstream style;
        style << "width:" << width << "px;";
        style << "height:" << height << "px";
        this->AppendAttribute(std::shared_ptr<Attribute>(new Attribute("style", style.str())));
        }
        else
        {
            this->AppendAttribute(std::shared_ptr<Attribute>(new Attribute("width", width)));
            this->AppendAttribute(std::shared_ptr<Attribute>(new Attribute("height", height)));
        }
    }

    virtual ~Image()
    {
#ifdef __DEBUG
        std::cout << "Destructing Image" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The Break class handles a break tag, <br>.
 */
class   Break : public Void
{
public:
    Break()
        : Void("br")
    {
#ifdef __DEBUG
        std::cout << "Constructing Break" << std::endl;
#endif
    }
    virtual ~Break()
    {
#ifdef __DEBUG
        std::cout << "Destructing Break" << std::endl;
#endif
    }
};

inline  std::shared_ptr<Break>   GetBreak()
{
    return std::make_shared<Break>();
}

//----------------------------------------------------------------------------
/**
 * @brief The Title class handles a one-line title node, <title></title>.
 */
class   Title : public NodeLine
{
public:
    Title(std::string text)
        : NodeLine("title", text)
    {
#ifdef __DEBUG
       std:: cout << "Constructing Title" << std::endl;
#endif
    }

    virtual ~Title()
    {
#ifdef __DEBUG
        std::cout << "Destructing Title" << std::endl;
#endif
    }
};

inline  std::shared_ptr<Title>   GetTitle(std::string text)
{
    return std::make_shared<Title>(text);
}

//----------------------------------------------------------------------------
/**
 * @brief The Heading class handles a one-line heading node, <h[level]></h[level]>.
 */
class   Heading : public NodeLine
{
public:
    Heading(std::string text, int level)
        : NodeLine("", text)
    {
        name = std::string("h") + std::to_string(level);

#ifdef __DEBUG
       std:: cout << "Constructing Heading" << std::endl;
#endif
    }

    virtual ~Heading()
    {
#ifdef __DEBUG
        std::cout << "Destructing Heading" << std::endl;
#endif
    }
};

inline  std::shared_ptr<Heading>   GetHeading(std::string text, int level)
{
    return std::make_shared<Heading>(text, level);
}

//----------------------------------------------------------------------------
/**
 * @brief The Text class handles a one-line node without tags representing text.
 */
class   Text : public NodeInline
{
public:
    Text(std::string text)
        : NodeInline("", text)
    {
#ifdef __DEBUG
        std::cout << "Constructing Text" << std::endl;
#endif
    }

    virtual ~Text()
    {
#ifdef __DEBUG
        std::cout << "Destructing Text" << std::endl;
#endif
    }

    virtual std::string Get(int indentation = 0) override
    {
        std::stringstream   stream;

        WriteIdentation(stream, indentation);
        stream << value;

        return stream.str();
    }
};

inline  std::shared_ptr<Text>   GetText(std::string text)
{
    return std::make_shared<Text>(text);
}

//----------------------------------------------------------------------------
/**
 * @brief The Span class handles a one-line Span node, <span></span>.
 */
class   Span : public NodeInline
{
public:
    Span()
        : NodeInline("span")
    {
#ifdef __DEBUG
       std:: cout << "Constructing Span" << std::endl;
#endif
    }

    Span(std::string text)
        : NodeInline("span", text)
    {
#ifdef __DEBUG
       std:: cout << "Constructing Span" << std::endl;
#endif
    }

    virtual ~Span()
    {
#ifdef __DEBUG
        std::cout << "Destructing Span" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The Div class handles a one-line Div node, <div></div>.
 */
class   Div : public NodeBase
{
public:
    Div()
        : NodeBase("div")
    {
#ifdef __DEBUG
       std:: cout << "Constructing Div" << std::endl;
#endif
    }

    virtual ~Div()
    {
#ifdef __DEBUG
        std::cout << "Destructing Div" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The Sub class handles a one-line subscript node, <sub></sub>.
 */
class   SubScript : public NodeInline
{
public:
    SubScript()
        : NodeInline("sub")
    {
#ifdef __DEBUG
       std:: cout << "Constructing SubScript" << std::endl;
#endif
    }

    SubScript(std::string text)
        : NodeInline("sub", text)
    {
#ifdef __DEBUG
       std:: cout << "Constructing SubScript" << std::endl;
#endif
    }

    virtual ~SubScript()
    {
#ifdef __DEBUG
        std::cout << "Destructing SubScript" << std::endl;
#endif
    }
};

inline  std::shared_ptr<SubScript>   GetSubScript()
{
    return std::make_shared<SubScript>();
}

inline  std::shared_ptr<SubScript>   GetSubScript(std::string text)
{
    return std::make_shared<SubScript>(text);
}

//----------------------------------------------------------------------------
/**
 * @brief The Sup class handles a one-line superscript node, <sup></sup>.
 */
class   SuperScript : public NodeInline
{
public:
    SuperScript()
        : NodeInline("sup")
    {
#ifdef __DEBUG
       std:: cout << "Constructing SuperScript" << std::endl;
#endif
    }

    SuperScript(std::string text)
        : NodeInline("sup", text)
    {
#ifdef __DEBUG
       std:: cout << "Constructing SuperScript" << std::endl;
#endif
    }

    virtual ~SuperScript()
    {
#ifdef __DEBUG
        std::cout << "Destructing SuperScript" << std::endl;
#endif
    }
};

inline  std::shared_ptr<SuperScript>   GetSuperScript()
{
    return std::make_shared<SuperScript>();
}

inline  std::shared_ptr<SuperScript>   GetSuperScript(std::string text)
{
    return std::make_shared<SuperScript>(text);
}

//----------------------------------------------------------------------------
/**
 * @brief The Paragraph class handles a pragraph node, <p></p>.
 */
class   Paragraph : public NodeBase
{
public:
    Paragraph()
        : NodeBase("p", "")
    {
#ifdef __DEBUG
       std:: cout << "Constructing Paragraph" << std::endl;
#endif
    }

    Paragraph(std::string text)
        : NodeBase("p", text)
    {
#ifdef __DEBUG
       std:: cout << "Constructing Paragraph" << std::endl;
#endif
    }

    virtual ~Paragraph()
    {
#ifdef __DEBUG
        std::cout << "Destructing Paragraph" << std::endl;
#endif
    }

    std::shared_ptr<NodeBase>   AppendText(std::string text)
    {
        return AppendChild(std::make_shared<Text>(text));
    }
};

inline  std::shared_ptr<Paragraph>   GetParagraph()
{
    return std::make_shared<Paragraph>();
}

inline  std::shared_ptr<Paragraph>   GetParagraph(std::string text)
{
    return std::make_shared<Paragraph>(text);
}

//----------------------------------------------------------------------------
/**
 * @brief The ListItem class handles a one-line list item node, <li></li>.
 */
class   ListItem : public NodeBase
{
public:
    ListItem()
        : NodeBase("li")
    {
#ifdef __DEBUG
        std::cout << "Constructing ListItem" << std::endl;
#endif
    }

    ListItem(std::string text)
        : NodeBase("li", text)
    {
#ifdef __DEBUG
        std::cout << "Constructing ListItem" << std::endl;
#endif
    }

    virtual ~ListItem()
    {
#ifdef __DEBUG
        std::cout << "Destructing ListItem" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The UnorderedList class handles an unordered list, <ul></ul>.
 */
class   UnorderedList : public NodeBase
{
public:
    UnorderedList()
        : NodeBase("ul")
    {
#ifdef __DEBUG
        std::cout << "Constructing UnorderedList" << std::endl;
#endif
    }

    virtual ~UnorderedList()
    {
#ifdef __DEBUG
        std::cout << "Destructing UnorderedList" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The OrderedList class handles an ordered list, <ol></ol>.
 */
class   OrderedList : public NodeBase
{
public:
    OrderedList()
        : NodeBase("ol")
    {
#ifdef __DEBUG
        std::cout << "Constructing OrderedList" << std::endl;
#endif
    }

    virtual ~OrderedList()
    {
#ifdef __DEBUG
        std::cout << "Destructing OrderedList" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The Table class  handles an ordered list, <table></table>.
 */
class   Table : public NodeBase
{
public:
    Table()
        : NodeBase("table")
    {
#ifdef __DEBUG
        std::cout << "Constructing Table" << std::endl;
#endif
    }

    Table(std::string caption)
        : NodeBase("table")
    {
        AppendChild(std::make_shared<NodeBase>("caption", caption));
#ifdef __DEBUG
        std::cout << "Constructing Table" << std::endl;
#endif
    }

    virtual ~Table()
    {
#ifdef __DEBUG
        std::cout << "Destructing Table" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The TableRow class handles a table row, <tr></tr>.
 */
class   TableRow : public NodeBase
{
public:
    TableRow()
        : NodeBase("tr")
    {
#ifdef __DEBUG
        std::cout << "Constructing TableRow" << std::endl;
#endif
    }

    virtual ~TableRow()
    {
#ifdef __DEBUG
        std::cout << "Destructing TableRow" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The TableElement class handles a one-line table element node, i.e. cell, <td>text</td>.
 */
class   TableElement : public NodeLine
{
public:
    TableElement()
        : NodeLine("td")
    {
#ifdef __DEBUG
        std::cout << "Constructing TableElement" << std::endl;
#endif
    }

    TableElement(std::string text)
        : NodeLine("td", text)
    {
#ifdef __DEBUG
        std::cout << "Constructing TableElement" << std::endl;
#endif
    }

    virtual ~TableElement()
    {
#ifdef __DEBUG
        std::cout << "Destructing TableElement" << std::endl;
#endif
    }
};

//----------------------------------------------------------------------------
/**
 * @brief The TableHeaderElement class handles a one line table header element node, <th>text</th>.
 */
class   TableHeaderElement : public NodeLine
{
public:
    TableHeaderElement()
        : NodeLine("th")
    {
#ifdef __DEBUG
        std::cout << "Constructing TableHeaderElement" << std::endl;
#endif
    }

    TableHeaderElement(std::string text)
        : NodeLine("th", text)
    {
#ifdef __DEBUG
        std::cout << "Constructing TableHeaderElement" << std::endl;
#endif
    }

    virtual ~TableHeaderElement()
    {
#ifdef __DEBUG
        std::cout << "Destructing TableHeaderElement" << std::endl;
#endif
    }
};

} // namespace simple_html

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#endif  // SIMPLE_HTML_WRITER_H
#include <string>
