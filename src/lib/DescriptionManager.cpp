/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QHashIterator>
#include "Description.h"
#include "DescriptionManager.h"


namespace BOI {


DescriptionManager::DescriptionManager()
    : DescriptionRegistrar(),
      m_descriptions()
{
}


DescriptionManager::~DescriptionManager()
{
    QHashIterator<QString, Description*> it(m_descriptions);

    while (it.hasNext())
    {
        it.next();
        delete it.value();
    }
}


Description* DescriptionManager::GetDescription(const QString& uuidMajor,
                                                const QString& uuidMinor)
{
    Description* pDescription = NULL;

    if (uuidMinor.isEmpty())
    {
        pDescription = m_descriptions.value(uuidMajor, NULL);
    }
    else
    {
        pDescription = m_descriptions.value(uuidMajor + uuidMinor, NULL);

        if (pDescription == NULL)
        {
            pDescription = m_descriptions.value(uuidMajor, NULL);
        }
    }

    return pDescription;
}


void DescriptionManager::RegisterDescription(Description* pDescription)
{
    QString key = pDescription->UuidMajor();

    if (!pDescription->UuidMinor().isEmpty())
    {
        key += pDescription->UuidMinor();
    }

    Description* pPrev = m_descriptions.value(key, NULL);
    if ((pPrev != NULL) && (pDescription != pPrev))
    {
        delete pPrev;
    }

    m_descriptions.insert(key, pDescription);
}


void DescriptionManager::InitStandardDescriptions()
{
    Description* pDescription = new Description();

    pDescription->SetUuidMajor("{84e256a6-9291-451b-a0ff-f38602d2f839}");
    pDescription->SetTitle("Set Position [Receiver]");
    pDescription->SetBody("Sets the position of the component in parent coordinates.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{af919167-3fd8-4fad-8990-cce279a8366c}");
    pDescription->SetTitle("Set Rotation [Receiver]");
    pDescription->SetBody("Sets the rotation of the component.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{0107f9b1-af4c-4b48-b772-c21c567cb53a}");
    pDescription->SetTitle("Set Opacity [Receiver]");
    pDescription->SetBody("Sets the opacity of the component."
                          "Values should be between 0.0 and 1.0 where 0.0 is transparent "
                          "and 1.0 is opaque.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{f4e6a67a-8cd5-441c-a7c0-367c8360a146}");
    pDescription->SetTitle("Set Visible [Receiver]");
    pDescription->SetBody("Sets the visibility of the component. Valid values are true and false.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{6fbe5587-9110-4b87-9049-2d5058d09304}");
    pDescription->SetTitle("Parent Bounding Box [Emitter]");
    pDescription->SetBody("Emits the bounding box of the component mapped into "
                          "its parent coordinate system.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{7473aebc-56e3-4dc4-994e-da426f45866c}");
    pDescription->SetTitle("Load Url [Receiver]");
    pDescription->SetBody("Loads the specified url.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{469a8be0-d607-48a0-981f-b41d7a034be5}");
    pDescription->SetTitle("Set HTML [Receiver]");
    pDescription->SetBody("Sets the HTML of the component to the specified string.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{84235b00-1b96-43bf-907b-240dfc74e6b0}");
    pDescription->SetTitle("Evaluate Javascript [Receiver]");
    pDescription->SetBody("Evaluates the passed in Javascript in the context of the current page.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{5f79f90a-0586-45f2-9c9e-53b3143fc475}");
    pDescription->SetTitle("Default Browser Emitter [Emitter]");
    pDescription->SetBody("Emits data from the javascript executing in the current page.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{7f2249e4-6c3c-40a5-9cff-59501f06ee37}");
    pDescription->SetTitle("Set Text [Receiver]");
    pDescription->SetBody("Sets the text of the component.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{b01ea5e1-a4b9-4114-873f-376ab652a554}");
    pDescription->SetTitle("Append Text [Receiver]");
    pDescription->SetBody("Adds text to the end of the text under the cursor.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{fb120d04-b81a-41cb-bb48-ca0295b4498f}");
    pDescription->SetTitle("Insert Text [Receiver]");
    pDescription->SetBody("Inserts text at the current cursor position.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{3518d488-5339-49a4-b927-bb69d7d9cf18}");
    pDescription->SetTitle("Text [Emitter]");
    pDescription->SetBody("Emits text from the component.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{898b92cc-bec5-42f3-ae9f-8dbd2b0b8af8}");
    pDescription->SetTitle("Start Timer [Receiver]");
    pDescription->SetBody("Starts the timer with the specified timeout interval in milliseconds.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{4be3297a-aea2-4e3c-85ef-6ca8dcb4fa10}");
    pDescription->SetTitle("Stop Timer [Receiver]");
    pDescription->SetBody("Stops the timer.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{8eff1256-171c-44b5-82a3-fa64d3625c31}");
    pDescription->SetTitle("Timeout [Emitter]");
    pDescription->SetBody("Emitted when the timer timeout interval has elapsed.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{8d001b1b-c057-44c8-a4a6-aab8e9da1916}");
    pDescription->SetTitle("Window Size [Emitter]");
    pDescription->SetBody("Emits the size of the window after it has changed.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{24a74700-21ae-4e99-8e95-366901620655}");
    pDescription->SetTitle("Window Width [Emitter]");
    pDescription->SetBody("Emits the width of the window after it has changed.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{64078be7-c9eb-4851-bf29-5f2484a81401}");
    pDescription->SetTitle("Window Height [Emitter]");
    pDescription->SetBody("Emits the height of the window after it has changed.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{24532bc9-ee2c-490c-b519-ea25844f32a0}");
    pDescription->SetTitle("Javascript Emitter 1 [Emitter]");
    pDescription->SetBody("The first javascript emitter.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{1123ddac-cf4b-4c3b-a0b2-0d1de01ed917}");
    pDescription->SetTitle("Javascript Emitter 2 [Emitter]");
    pDescription->SetBody("The second javascript emitter.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{a803a200-109e-4511-a9cf-f13d2a62da70}");
    pDescription->SetTitle("Javascript Emitter 3 [Emitter]");
    pDescription->SetBody("The third javascript emitter.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{0f2b6164-bef3-4e2e-b2a1-754cf2e17dd4}");
    pDescription->SetTitle("Javascript Emitter 4 [Emitter]");
    pDescription->SetBody("The fourth javascript emitter.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{7f26d820-effc-4fe3-8553-1a57cd88244d}");
    pDescription->SetTitle("Javascript Receiver Func 1 [Receiver]");
    pDescription->SetBody("The first javascript receiver.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{eff42c37-57dd-4f1f-95b2-16549eff87a8}");
    pDescription->SetTitle("Javascript Receiver Func 2 [Receiver]");
    pDescription->SetBody("The second javascript receiver.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{4bf972ee-26d3-4afe-b38b-71d93c1bb514}");
    pDescription->SetTitle("Javascript Receiver Func 3 [Receiver]");
    pDescription->SetBody("The third javascript receiver.");

    RegisterDescription(pDescription);

    pDescription = new Description();
    pDescription->SetUuidMajor("{c00548bc-8792-48b9-a21f-4393b790da17}");
    pDescription->SetTitle("Javascript Receiver Func 4 [Receiver]");
    pDescription->SetBody("The fourth javascript receiver.");

    RegisterDescription(pDescription);
}


} // namespace BOI

