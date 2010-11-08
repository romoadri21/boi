/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QHashIterator>
#include "MenuItem.h"
#include "ProfileType.h"
#include "StandardMenus.h"
#include "StandardActions.h"
#include "StandardComponents.h"
#include "MenuManager.h"


namespace BOI {


MenuManager::MenuManager()
    : MenuRegistrar(),
      m_menus(),
      m_profiles(),
      m_cache()
{
}


MenuManager::~MenuManager()
{
    /*
     * Delete all the menu items.
     */

    QHashIterator<QString, MenuItem*> mit(m_menus);

    while (mit.hasNext())
    {
        mit.next();
        MenuItem* pItem = mit.value();

        while (pItem != NULL)
        {
            pItem = pItem->Delete();
        }
    }

    /*
     * Delete all the profiles.
     */

    QHashIterator<QString, Profile*> pit(m_profiles);

    while (pit.hasNext())
    {
        pit.next();
        Profile* pProfile = pit.value();

        while (pProfile != NULL)
        {
            Profile* pNext = pProfile->pNext;
            delete pProfile;
            pProfile = pNext;
        }
    }

    /*
     * Delete the MenuSets stored in the cache.
     */

    QHashIterator<int, MenuSet*> cit(m_cache);

    while (cit.hasNext())
    {
        cit.next();
        delete cit.value();
    }
}


void MenuManager::InitStandardMenus()
{
    /*
     * Root menu.
     */

    ActionArgs timerArgs;
    timerArgs.Set("UUID", BOI_UUID_C(Timer));

    ActionArgs browserArgs;
    browserArgs.Set("UUID", BOI_UUID_C(Browser));

    ActionArgs windowSizeArgs;
    windowSizeArgs.Set("UUID", BOI_UUID_C(WindowSize));

    ActionArgs javascriptArgs;
    javascriptArgs.Set("UUID", BOI_UUID_C(Javascript));

    ActionArgs verticalLayoutArgs;
    verticalLayoutArgs.Set("UUID", BOI_UUID_C(VerticalLayout));

    ActionArgs horizontalLayoutArgs;
    horizontalLayoutArgs.Set("UUID", BOI_UUID_C(HorizontalLayout));

    MenuItem* pRootItem = new MenuItem("View");

    pRootItem
        -> SetChild("Pan", BOI_UUID_A(Pan))
        -> SetNext("Zoom", BOI_UUID_A(Zoom))
            -> SetChild("Interactive", BOI_UUID_A(Zoom))
            -> SetNext("All", BOI_UUID_A(FitAllInView))
            -> SetNext("1:1", BOI_UUID_A(ResetViewScale)) -> Parent()
        -> SetNext("Reset", BOI_UUID_A(ResetView))
        -> SetNext("Toggle Fullscreen", BOI_UUID_A(ToggleFullScreen))
        -> SetNext("Add SubView") -> Parent()
    -> SetNext("Insert")
        -> SetChild("Window Size", BOI_UUID_A(Insert), windowSizeArgs) // TODO: maybe put this in the "view" sub category
        -> SetNext("Layout")
            -> SetChild("Horizontal", BOI_UUID_A(Insert), horizontalLayoutArgs)
            -> SetNext("Vertical", BOI_UUID_A(Insert), verticalLayoutArgs) -> Parent()
        -> SetNext("Collection", BOI_UUID_A(InsertCollection))
        -> SetNext("Text", BOI_UUID_A(InsertText))
        -> SetNext("Timer", BOI_UUID_A(Insert), timerArgs)
        -> SetNext("Browser", BOI_UUID_A(Insert), browserArgs)
        -> SetNext("Javascript", BOI_UUID_A(Insert), javascriptArgs) -> Parent()
    -> SetNext("Show")
        -> SetChild("All")
        -> SetNext("Area") -> Parent()
    -> SetNext("State")
    -> SetNext("Paste")
    -> SetNext("Selection")
        -> SetChild("Set (All)")
        -> SetNext("Set (Rect)", BOI_UUID_A(RectSelect))
        -> SetNext("Remove From")
        -> SetNext("Add To")
        -> SetNext("Clear", BOI_UUID_A(ClearSelection))
        -> SetNext("Invert") -> Parent()
    -> SetNext("System")
        -> SetChild("Shutdown", BOI_UUID_A(Shutdown));

    RegisterMenu(BOI_UUID_M(Root), pRootItem);

    /*
     * Standard Component menu.
     */

    ActionArgs connectInputArgs;
    connectInputArgs.Set("Type", ProfileType_Input);

    ActionArgs connectOutputArgs;
    connectOutputArgs.Set("Type", ProfileType_Output);

    pRootItem = new MenuItem("Disconnect");
    pRootItem
    -> SetNext("Connect")
        -> SetChild("Input", BOI_UUID_A(Connect), connectInputArgs)
        -> SetNext("Output", BOI_UUID_A(Connect), connectOutputArgs) -> Parent()
    -> SetNext("Transform")
        -> SetChild("Move")
        -> SetNext("Rotate")
            -> SetChild("Set")
                -> SetChild("0", BOI_UUID_A(SetRotation))
                -> SetNext("45")
                -> SetNext("90")
                -> SetNext("135")
                -> SetNext("180")
                -> SetNext("225")
                -> SetNext("270")
                -> SetNext("315") -> Parent()
            -> SetNext("Free", BOI_UUID_A(FreeRotate))
            -> SetNext("+45")
            -> SetNext("-45")
            -> SetNext("+90")
            -> SetNext("-90")
            -> SetNext("+180")
            -> SetNext("-180") -> Parent() -> Parent()
    -> SetNext("Delete", BOI_UUID_A(Delete))
    -> SetNext("Hide")
    -> SetNext("Stack")
        -> SetChild("On Top")
        -> SetNext("After")
        -> SetNext("Before") -> Parent()
    -> SetNext("Collect", BOI_UUID_A(Collect))
    -> SetNext("Parent")
        -> SetChild("Set Child", BOI_UUID_A(SetChild))
        -> SetNext("UnSet") -> Parent()
    -> SetNext("Copy");

    RegisterMenu(BOI_UUID_M(Component), pRootItem);

    /*
     * Resize menu.
     */

    ActionArgs xResizeArgs;
    xResizeArgs.Set("Axis", "x");

    ActionArgs yResizeArgs;
    yResizeArgs.Set("Axis", "y");

    ActionArgs xyResizeArgs;
    xyResizeArgs.Set("Axis", "xy");

    pRootItem = new MenuItem("Resize");
    pRootItem
        -> SetChild("X", BOI_UUID_A(Resize), xResizeArgs)
        -> SetNext("Y",  BOI_UUID_A(Resize), yResizeArgs)
        -> SetNext("XY", BOI_UUID_A(Resize), xyResizeArgs);

    RegisterMenu(BOI_UUID_M(Resize), pRootItem);

    /*
     * Layout menu.
     */

    pRootItem = new MenuItem("Layout");
    pRootItem
        -> SetChild("Add To", BOI_UUID_A(AddToLayout))
        -> SetNext("Remove From", BOI_UUID_A(RemoveFromLayout));

    RegisterMenu(BOI_UUID_M(Layout), pRootItem);

    /*
     * Browser menu.
     */

    pRootItem = new MenuItem("Load Url", BOI_UUID_A(LoadUrl));

    RegisterMenu(BOI_UUID_M(Browser), pRootItem);

    /*
     * Text menu.
     */

    pRootItem = new MenuItem("Append", BOI_UUID_A(AppendText));
    pRootItem
    -> SetNext("Insert", BOI_UUID_A(PutText));

    RegisterMenu(BOI_UUID_M(Text), pRootItem);
}


void MenuManager::InitStandardProfiles()
{
    QStringList uuids;
    QStringList menus;

    /*
     * Register the profile
     * for the Resize menu.
     */

    uuids.append("{acb9a365-42b3-47b9-a2a7-a7c5f1999b0f}");
    menus.append(BOI_UUID_M(Resize));

    RegisterProfile(uuids, menus);

    /*
     * Register the profile
     * for the Layout menu.
     */

    uuids.clear();
    menus.clear();

    uuids.append("{6a7ab00f-1ab4-4324-9eb4-e614bfca4a16}");
    uuids.append("{b11a0db4-cb96-4bf6-9631-fd96f20ea6ab}");
    uuids.append("{f742f223-bb7b-48f0-92a8-81702e14de16}");
    menus.append(BOI_UUID_M(Layout));

    RegisterProfile(uuids, menus);

    /*
     * Register the profile
     * for the Browser menu.
     */

    uuids.clear();
    menus.clear();

    uuids.append("{469a8be0-d607-48a0-981f-b41d7a034be5}");
    uuids.append("{7473aebc-56e3-4dc4-994e-da426f45866c}");
    uuids.append("{84235b00-1b96-43bf-907b-240dfc74e6b0}");
    menus.append(BOI_UUID_M(Browser));

    RegisterProfile(uuids, menus);

    /*
     * Register the profile
     * for the Text menu.
     */

    uuids.clear();
    menus.clear();

    uuids.append("{7f2249e4-6c3c-40a5-9cff-59501f06ee37}");
    uuids.append("{b01ea5e1-a4b9-4114-873f-376ab652a554}");
    uuids.append("{c768ae35-9c07-46d8-be25-3ad3f65ccdfd}");
    uuids.append("{fb120d04-b81a-41cb-bb48-ca0295b4498f}");
    menus.append(BOI_UUID_M(Text));

    RegisterProfile(uuids, menus);
}


MenuItem* MenuManager::GetMenu(const QString& uuid)
{
    return m_menus.value(uuid, NULL);
}


bool MenuManager::RegisterMenu(const QString& uuid, MenuItem* pItem)
{
    /*
     * It is the responsibility of the calling code
     * to assure that no menu is left stranded if
     * a menu already exists with the specified uuid.
     */

    m_menus.insert(uuid, pItem);
    return true;
}


void MenuManager::RegisterProfile(const QStringList& uuids,
                                  const QStringList& menus)
{
    if (uuids.isEmpty() || menus.isEmpty()) return;

    bool exactMatch;

    Profile* pProfile = GetProfile(uuids, &exactMatch);

    if (pProfile == NULL)
    {
        pProfile = new Profile;
        pProfile->pNext = NULL;
        pProfile->menus = menus;
        pProfile->uuids = uuids;

        m_profiles.insert(uuids.first(), pProfile);
    }
    else if (!exactMatch)
    {
        Profile* pNewProfile = new Profile;
        pNewProfile->pNext = NULL;
        pNewProfile->menus = menus;
        pNewProfile->uuids = uuids;

        pProfile->pNext = pNewProfile;
    }
    else // Exact profile match found
    {
        pProfile->menus += menus;
        pProfile->menus.removeDuplicates();
    }
}


void MenuManager::GetMenus(const QStringList& profile,
                           MenuSet& menuSet,
                           int profileId)
{
    menuSet.Clear();

    for (int i=0; i < profile.size(); i++)
    {
        Profile* pProfile = m_profiles.value(profile.at(i), NULL);

        while (pProfile != NULL)
        {
            if (Contains(profile, pProfile->uuids, i))
            {
                if (pProfile->menuSet.NumMenus() == 0)
                {
                    FillMenuSet(&pProfile->menuSet, pProfile->menus);
                }

                menuSet.Append(pProfile->menuSet);
            }

            pProfile = pProfile->pNext;
        }
    }

    // TODO: remove duplicate menu items
    // from menuSet? Or combine them?

    if (profileId != -1)
    {
        MenuSet* pMenuSet = new MenuSet;
        *pMenuSet = menuSet;

        m_cache.insert(profileId, pMenuSet);
    }
}


bool MenuManager::GetMenus(int profileId, MenuSet& menuSet)
{
    MenuSet* pMenuSet = m_cache.value(profileId, NULL);

    if (pMenuSet != NULL)
    {
        menuSet = *pMenuSet;
        return true;
    }

    return false;
}


MenuManager::Profile* MenuManager::GetProfile(const QStringList& uuids,
                                              bool* exactMatch)
{
    Profile* pProfile = m_profiles.value(uuids.first(), NULL);

    if (pProfile != NULL)
    {
        do
        {
            if (pProfile->uuids == uuids)
            {
                *exactMatch = true;
                return pProfile;
            }
            else if (pProfile->pNext == NULL)
            {
                *exactMatch = false;
                return pProfile;
            }

            pProfile = pProfile->pNext;

        } while (true);
    }

    return NULL;
}


bool MenuManager::Contains(const QStringList& list1,
                           const QStringList& list2,
                           int startIndex1,
                           int startIndex2)
{
    int i = startIndex1;
    int j = startIndex2;

    int numMatches = 0;

    while ((i < list1.size()) &&
           (j < list2.size()))
    {
        int result = list1.at(i).compare(list2.at(j));

        if (result == 0)
        {
            i++;
            j++;
            numMatches++;
        }
        else if (result > 0) j++;
        else i++;
    }

    if (numMatches == (list2.size() - startIndex2))
    {
        return true;
    }

    return false;
}


void MenuManager::FillMenuSet(MenuSet* pMenuSet, const QStringList& menuUuids)
{
    for (int i=0; i < menuUuids.size(); i++)
    {
        MenuItem* pItem = m_menus.value(menuUuids.at(i), NULL);

        if (pItem != NULL)
        {
            pMenuSet->Append(pItem);
        }
    }
}


} // namespace BOI

