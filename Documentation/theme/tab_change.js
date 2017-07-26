window.onload = function()
{
	var elements = document.getElementsByTagName("div");
	for(var i = 0; i < elements.length;i++)
	{
		
		if (elements[i].className == "ln_lang_tabs")
		{
			lnInitLangTab(elements[i])
			
			lnSelectTab(elements[i], "tab1");
			
		}
	}
}

function lnGetContentsElement(element)
{
	var elements = [];
	var children = element.childNodes;
	for(var i = 0; i < children.length;i++)
	{
		if (children[i].className == "ln_tab_content")
		{
			elements.push(children[i]);
		}
	}
	return elements;
}

function lnGetContentByName(contents, name)
{
	for(var i = 0; i < contents.length;i++)
	{
		if (contents[i].getAttribute("name") == name)
		{
			return contents[i];
		}
	}
	return null;
}

function lnInitLangTab(element)
{
	var childContents = lnGetContentsElement(element);
	var lis = element.getElementsByTagName("li");
	for(var i = 0; i < lis.length;i++)
	{
		var contentName = lis[i].getAttribute("name");
		var content = lnGetContentByName(childContents, contentName);
		lis[i].onclick = function()
		{
			var elements = document.getElementsByTagName("div");
			for(var i = 0; i < elements.length;i++)
			{
				if (elements[i].className == "ln_lang_tabs")
				{
					lnSelectTab(elements[i], this.getAttribute("name"));
				}
			}
		}
	}
}

// element : <div class="ln_lang_tabs">
function lnSelectTab(element, contentName)
{
	//alert("lnSelectTab");
	var childContents = lnGetContentsElement(element);
	var lis = element.getElementsByTagName("li");
	
	//alert("childContents.length:" + childContents.length);
	//alert("lis.length:" + lis.length);
	
	for(var i = 0; i < lis.length;i++)
	{
		var name = lis[i].getAttribute("name");
		var content = lnGetContentByName(childContents, name);
		//alert(name+":"+contentName);
		if (name)
		{
			if (name == contentName)
			{
				content.style.display		= "block";
				content.style.visibility	= "visible";
				lis[i].className		= "active";
			}
			else
			{
			
				content.style.display		= "none";
				content.style.visibility	= "hidden";
				lis[i].className			= "";
			}
		}
	}
}
