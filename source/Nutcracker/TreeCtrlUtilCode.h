/********************************************************************
	CrazyGaze (http://www.crazygaze.com)
	Author : Rui Figueira
	Email  : rui@crazygaze.com
	
	purpose:
	Miscelaneous utility code for tree controls
*********************************************************************/

#pragma once

namespace nutcracker
{

namespace TreeCtrlUtil
{

template<class TreeItemType>
void iterateTreeItems(wxTreeCtrl* ctrl, wxTreeItemId start, const std::function<void(wxTreeItemId itemid, TreeItemType*)>& func)
{
	if (!start.IsOk())
		return;
	wxTreeItemIdValue cookie;
	auto itemid = ctrl->GetFirstChild(start, cookie);
	while(itemid.IsOk())
	{
		auto data = ctrl->GetItemData(itemid);
		func(itemid, data ? static_cast<TreeItemType*>(data) : nullptr);

		iterateTreeItems<TreeItemType>(ctrl, itemid, func);

		itemid = ctrl->GetNextChild(start, cookie);
	}
}

/*
Uniquely identifies a tree item.
It's up to the user to set the type and id values any way necessary according to the code using this
*/
struct TreeItemID
{
	TreeItemID(cz::u32 type_=0, cz::u32 id0 = 0, cz::u32 id1 = 0, cz::u32 id2 = 0, cz::u32 id3=0);
	cz::u32 itemtype;
	cz::u32 id[4];

	bool isValid() const;
	/**
	 * Comparison operator so it's possible to use it as a map key
	 */
	bool operator<(const TreeItemID& other) const;
	bool operator==(const TreeItemID& other) const;
};

/**
 * User code should inherit from this class to add custom data to the tree
 */
class TreeItemCustomData
{
public:
	TreeItemCustomData() { }
	virtual ~TreeItemCustomData() { }
};

/**
* Tree item data
* This is saved outside the TreeItem itself, so we can save the layout, for when we refresh the tree control
*/
class TreeItemData
{
public:
	explicit TreeItemData(wxTreeCtrl* parent, const TreeItemID id_ = TreeItemID());
	virtual ~TreeItemData();

	const wxTreeItemId& getWxTreeItemId();
	const TreeItemID& getItemId() const;
	const std::shared_ptr<TreeItemCustomData>& getCustomData();
	void setCustomData(const std::shared_ptr<TreeItemCustomData>& customdata);
	void setImage(int image, wxTreeItemIcon which = wxTreeItemIcon_Normal);
	void expand();
	void setName(const wxString& name);

protected:
	void retrieveLayout();
	friend class TreeCtrlData;

	TreeItemID m_id;
	wxTreeItemId m_wxTreeItemId;
	wxTreeCtrl* m_ctrl;
	bool m_expanded = false;
	bool m_selected = false;
	bool m_visible = false;
	std::shared_ptr<TreeItemCustomData> m_customdata;
};


/**
 * Actual tree item.
 * It doesn't contain any data itself, but points to the data.
 * This allows saving the layout and data, making it easier to refresh the tree keeping the same layout
 */
struct TreeItemBase : public wxTreeItemData
{
	TreeItemBase() { }
	explicit TreeItemBase(std::shared_ptr<TreeItemData> data_) : data(data_) {}
	std::shared_ptr<TreeItemData> data;
};


/**
 * Puts everything together
 */
class TreeCtrlData
{
public:
	TreeCtrlData() {}
	explicit TreeCtrlData(wxFrame* parentFrame, wxTreeCtrl* ctrl);
	TreeCtrlData(const TreeCtrlData& other) = default;
	virtual ~TreeCtrlData();
	const std::shared_ptr<TreeItemData>& get(const TreeItemID& id);

	template<class T>
	std::shared_ptr<T> getItemCustomData(const TreeItemID& id)
	{
		auto item = get(id);
		if (!item || !item->getCustomData())
			return nullptr;
		return std::static_pointer_cast<T>(item->getCustomData());
	}

	std::shared_ptr<TreeItemData> findByWxTreeItemId(const wxTreeItemId& id);
	wxTreeCtrl* getCtrl();

	void iterateItems(const std::function<void(wxTreeItemId itemid, TreeItemBase*)>& func);
	void clear();

	void startRefresh();
	std::shared_ptr<TreeItemData> setupRoot(const TreeItemID& id, int image);
	std::shared_ptr<TreeItemData> addItem(const std::shared_ptr<TreeItemData>& parentItem, const TreeItemID& id, const cz::UTF8String& text, int image);
	void endRefresh();
protected:
	void retrieveLayout();

	std::map<TreeItemID, std::shared_ptr<TreeItemData>> m_layout;
	std::map<TreeItemID, std::shared_ptr<TreeItemData>> m_templayout;
	bool m_layoutprune = false;
	wxFrame* m_frame = nullptr;
	wxTreeCtrl* m_ctrl = nullptr;
	// updated when refreshing the tree
	wxTreeItemId m_selectedId;
	// using a TreeItemID instead of wxTreeItemID, because once we delate all the tree items,
	// wxTreeItemId values are invalidated
	TreeItemID m_firstVisibleId; 
	wxTreeItemId m_firstWxVisibleId;
	// used to collect code to run we finish refreshing the tree
	std::vector<std::function<void()>> m_later;

	/*
	struct ScrollPos
	{
		int xpixunit, ypixunit, xview, yview, xsize, ysize, vxsize, vysize, newy, maxy, totalsize, thumbsize;
	} m_scrollpos;
	*/
};


} // namespace TreeCtrlUtil

} // namespace nutcracker


