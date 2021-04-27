

#include "../public/ServiceManagerEditor.h"


namespace marengine {


    template<EEditorContextType TEditorType>
    void FServiceManagerEditor::create(FServiceLocatorEditor* pServiceLocator) {

    }


    template<>
    void FServiceManagerEditor::create<EEditorContextType::IMGUI>(FServiceLocatorEditor* pServiceLocator);


}