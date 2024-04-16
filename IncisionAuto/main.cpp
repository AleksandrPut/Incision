
#include <NXOpen/Session.hxx>
#include <NXOpen/Part.hxx>


#include <NXOpen/Drawings_SectionView.hxx>
#include <NXOpen/Drawings_SectionViewBuilder.hxx>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Annotations_BreakSettingsBuilder.hxx>
#include <NXOpen/Annotations_DimensionStyleBuilder.hxx>
#include <NXOpen/Annotations_ForeshorteningSymbolSettingsBuilder.hxx>
#include <NXOpen/Annotations_FrameBarElementStyleBuilder.hxx>
#include <NXOpen/Annotations_FrameBarStyleBuilder.hxx>
#include <NXOpen/Annotations_HatchStyleBuilder.hxx>
#include <NXOpen/Annotations_HoleCalloutSettingsBuilder.hxx>
#include <NXOpen/Annotations_LetteringStyleBuilder.hxx>
#include <NXOpen/Annotations_LineArrowStyleBuilder.hxx>
#include <NXOpen/Annotations_Note.hxx>
#include <NXOpen/Annotations_OrdinateStyleBuilder.hxx>
#include <NXOpen/Annotations_RadialStyleBuilder.hxx>
#include <NXOpen/Annotations_SingleSidedDisplayBuilder.hxx>
#include <NXOpen/Annotations_StyleBuilder.hxx>
#include <NXOpen/Annotations_SymbolStyleBuilder.hxx>
#include <NXOpen/Annotations_UnitsStyleBuilder.hxx>
#include <NXOpen/DisplayableObject.hxx>
#include <NXOpen/Drafting_BaseEditSettingsBuilder.hxx>
#include <NXOpen/Drafting_SettingsManager.hxx>
#include <NXOpen/Drawings_EditViewLabelSettingsBuilder.hxx>
#include <NXOpen/Drawings_ViewCommonViewLabelBuilder.hxx>
#include <NXOpen/Drawings_ViewDetailLabelBuilder.hxx>
#include <NXOpen/Drawings_ViewLabelBuilder.hxx>
#include <NXOpen/Drawings_ViewProjectedLabelBuilder.hxx>
#include <NXOpen/Drawings_ViewSectionLabelBuilder.hxx>
#include <NXOpen/MenuBar_ContextMenu.hxx>
#include <NXOpen/MenuBar_ContextMenuEntry.hxx>
#include <NXOpen/MenuBar_ContextMenuProperties.hxx>
#include <NXOpen/MenuBar_MenuBarManager.hxx>
#include <NXOpen/MenuBar_MenuButton.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Selection.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/TaggedObject.hxx>

#include <NXOpen/NXObjectManager.hxx>
#include <NXOpen/Drawings_EditViewSettingsBuilder.hxx>
#include <NXOpen/AttributePropertiesBuilder.hxx>


#include <uf_defs.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/AttributeManager.hxx>
#include <NXOpen/AttributePropertiesBaseBuilder.hxx>
#include <NXOpen/AttributePropertiesBuilder.hxx>
#include <NXOpen/BasePart.hxx>
#include <NXOpen/Builder.hxx>
#include <NXOpen/DateBuilder.hxx>
#include <NXOpen/DateItemBuilder.hxx>
#include <NXOpen/DateItemBuilderList.hxx>
#include <NXOpen/Drawings_DraftingViewCollection.hxx>
#include <NXOpen/Drawings_SectionView.hxx>
#include <NXOpen/Expression.hxx>
#include <NXOpen/MenuBar_ContextMenuProperties.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/ObjectGeneralPropertiesBuilder.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/PropertiesManager.hxx>
#include <NXOpen/SelectNXObjectList.hxx>
#include <NXOpen/Selection.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Update.hxx>

#include <stdio.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_defs.h>
#include <uf_drf.h>
#include <uf_curve.h>

#include <uf_view.h>
#include <uf_draw.h>
#include <uf_obj.h>

#include <iostream>
#include <string>
#include <vector>

#include <NXSigningResource.cpp>
#include <exception>


using namespace NXOpen;

void rename_label(std::string str_num, std::string str_sumbol, tag_t view_tag, bool check);

void search_name()
{
    //UF_UI_open_listing_window();
    int         ifail = 0;
    int num_drawings = 0, num_views = 0;
    tag_p_t Drawing_tags = NULL, view_list = NULL;
    char name_draw[UF_OBJ_NAME_LEN + 1];
    char name[UF_OBJ_NAME_LEN + 1];
    UF_VIEW_type_t type;
    UF_VIEW_subtype_t subtype;
    tag_t view_label_tag = NULL_TAG;
    UF_DRF_label_info_t* label_info;
    UF_DRAW_view_label_parms_t view_label_parms;
    char* string_value;
    logical has_attribute;

    //Ќаходим все листы чертежа
    UF_DRAW_ask_drawings(&num_drawings, &Drawing_tags);
    for (int i = 0; i < num_drawings; ++i)
    {
        // ѕолучаем список тегов вида дл€ n чертежа 
        UF_DRAW_ask_views(Drawing_tags[i], &num_views, &view_list);
        UF_OBJ_ask_name(Drawing_tags[i], name_draw);

        std::string str_num_draw = name_draw;
        auto slashPosition4 = str_num_draw.find_first_of(" "); // начина€ с 11 плюсов
        //strin4.erase(strin4.begin() + slashPosition4 + 1, strin4.end()); // с начала
        str_num_draw.erase(str_num_draw.begin(), str_num_draw.begin() + slashPosition4); //с конца
        //”дал€ем лишние пробелы из строки
        str_num_draw.erase(remove(str_num_draw.begin(), str_num_draw.end(),' '), str_num_draw.end()); //с конца
        //name_drawing.push_back(name_draw);
        for (int x = 0; x < num_views; ++x)
        {
            //ѕолучаем тип и подтип дл€ каждого вида
            UF_VIEW_ask_type(view_list[x], &type, &subtype);
            if (type == UF_VIEW_DRAWING_MEMBER_TYPE && subtype == UF_VIEW_SECTION_SUBTYPE)//UF_VIEW_DRAWING_MEMBER_TYPE = 1 UF_VIEW_SECTION_SUBTYPE = 0
            {
                //ѕолучаем тег надписи разреза
                UF_DRAW_ask_view_label(view_list[x], &view_label_tag);

                UF_DRAW_ask_view_label_parms(view_label_tag, &view_label_parms);
                UF_ATTR_get_string_user_attribute(view_list[x], "VWLETTER_DISP", UF_ATTR_NOT_ARRAY, &string_value, &has_attribute);

                std::string str = (std::string)string_value;
                std::string copy_str, str_2;
                for (int i = 0; i < str.size(); ++i)
                {
                    if (str[i] == '<')
                        break;
                    copy_str[i] = str[i];
                }
                strcpy(const_cast<char*>(str_2.c_str()), copy_str.c_str());

                bool check = false;
                tag_t sxline_tag;
                UF_DRAW_ask_sxline_of_sxview(view_list[x], &sxline_tag);
                double step_dir[3];
                double arrow_dir[3];
                tag_t pview_tag;
                int num_sxviews;
                tag_p_t sxview_tags;
                int num_sxsegs;
                tag_p_t sxseg_tags;
                UF_DRAW_sxline_status_t sxline_status;
                //UF_DRAW_ask_unfolded_sxline
                UF_DRAW_ask_simple_sxline(sxline_tag, step_dir, arrow_dir, &pview_tag, &num_sxviews, &sxview_tags, &num_sxsegs, &sxseg_tags, &sxline_status);
                char name[UF_OBJ_NAME_LEN + 1];
                tag_t im;
                UF_DRAW_ask_drawing_of_view(pview_tag, &im);
                UF_OBJ_ask_name(im, name);
                
                
                std::string str_num_draw1 = name;
                auto slashPosition = str_num_draw1.find_first_of(" "); // начина€ с 11 плюсов
                //strin4.erase(strin4.begin() + slashPosition4 + 1, strin4.end()); // с начала
                str_num_draw1.erase(str_num_draw1.begin(), str_num_draw1.begin() + slashPosition); //с конца
                //”дал€ем лишние пробелы из строки
                str_num_draw1.erase(remove(str_num_draw1.begin(), str_num_draw1.end(), ' '), str_num_draw1.end()); //с конца

                //ѕроверка если на одном листе то не запускать
                if (str_num_draw != str_num_draw1)
                    rename_label(str_num_draw, str_2, view_list[x], check);

                UF_free(sxview_tags);
                UF_free(sxseg_tags);
            }
        }
    }
    UF_free(Drawing_tags);
}



void rename_label(std::string str_num, std::string str_sumbol, tag_t view_tag, bool check)
{
    Session* theSession = Session::GetSession();
    Part* workPart(theSession->Parts()->Work());
    Part* displayPart(theSession->Parts()->Display());

    tag_t view_label_tag;
    char* string_value;
    char* string_value2;
    char string_label_value[200];
    logical has_attribute;
    //logical has_attribute2;
    std::string new_labelSTR, new_labelSTR_2;
    //auto index = std::find(tag_node.begin(), tag_node.end(), columnID);
    //tag_t view_tag = tag_label[index - tag_node.begin()];
    //auto index_pref = find(prefix_label.begin(), prefix_label.end(), columnID + 1);
    //unsigned int pref = prefix_label[index_pref - prefix_label.begin()];
    try
    {
       // NXString nstr(editedText.GetLocaleText(), NXString::UTF8);
        //ѕолучаем тег надписи разреза
        UF_DRAW_ask_view_label(view_tag, &view_label_tag);

        //ѕолучаем значение атрибута с именем "VWLETTER_DISP"
        UF_ATTR_get_string_user_attribute(view_tag, "VWLETTER_DISP", UF_ATTR_NOT_ARRAY, &string_value, &has_attribute);

        std::string strin0 = (std::string)string_value;
        auto slashPosition0 = strin0.find_first_of("<"); // начина€ с 11 плюсов
        //strin.erase(strin.begin() + slashPosition, strin.end()); // с начала
        strin0.erase(strin0.begin(), strin0.begin() + slashPosition0); //с конца

        std::string strin4 = strin0;
        auto slashPosition4 = strin4.find_first_of(">"); // начина€ с 11 плюсов
        strin4.erase(strin4.begin() + slashPosition4 + 1, strin4.end()); // с начала
        //strin4.erase(strin4.begin(), strin4.begin() + slashPosition4); //с конца


        std::string strin5 = (std::string)string_value;
        auto slashPosition5 = strin5.find_first_of("("); // начина€ с 11 плюсов
        strin5.erase(strin5.begin() + slashPosition5 + 1, strin5.end()); // с начала
        //strin5.erase(strin5.begin(), strin5.begin() + slashPosition5); //с конца

        std::string skob;
        bool check_s = false;
        for (int i = 0; i < strin5.size(); i++)
        {
            if (strin5[i] == '(')
                check_s = true;
            if (strin5[i] != '(')
                skob = strin5[i];
        }

        
        /*
        UF_UI_write_listing_window(("string_value - " + std::string(string_value)).c_str());
        UF_UI_write_listing_window("\n");
        UF_UI_write_listing_window(("strin5 - " + std::string(strin5)).c_str());
        UF_UI_write_listing_window("\n");
        UF_UI_write_listing_window(("strin0 - " + std::string(strin0)).c_str());
        UF_UI_write_listing_window("\n");
        UF_UI_write_listing_window(("strin4 - " + std::string(strin4)).c_str());
        UF_UI_write_listing_window("\n");*/

        


        std::string str_1 = "(" + str_num + ")";
        //ћен€ем и копируем
        if (!check_s)
        {
            //new_labelSTR = str_sumbol.c_str() + str_1 + strin0;// strin4 + "<L>";
            new_labelSTR = str_sumbol.c_str() + str_1 + strin0;// strin4 + "<L>";
        }
        else
        {
            new_labelSTR = skob + str_1 + strin0;// strin4 + "<L>";
        }
        
        
        //new_labelSTR = "ј1" + str_1 + strin0;

        strcpy(string_value, new_labelSTR.c_str());

        std::string str = str_sumbol;
        std::string copy_symbol_str;
        std::string copy_str;
        int count = 0;
        for (int i = 1; i < str.size(); ++i)
        {
            copy_str[count] = str[i];
            count++;
        }
        //for (int i = 0; i < pref; ++i)
        //{
        copy_symbol_str[0] = str[0];
        //}

        //strcpy(string_value2, (char*)str[0]);
        NXString nstr2(copy_str, NXString::Locale);
        NXString nstr3(copy_symbol_str, NXString::Locale);
        bool start = true;
        try
        {
            //«амена символа на отоброжаемой иерархии 
            std::vector<NXOpen::View*> views1;
            NXOpen::Drawings::SectionView* sectionView1(dynamic_cast<NXOpen::Drawings::SectionView*>(NXObjectManager::Get(view_tag)));

            views1.push_back(sectionView1);
            NXOpen::Drawings::EditViewSettingsBuilder* editViewSettingsBuilder2;
            editViewSettingsBuilder2 = workPart->SettingsManager()->CreateDrawingEditViewSettingsBuilder(views1);

            std::vector<NXOpen::Drafting::BaseEditSettingsBuilder*> editsettingsbuilders2;
            editsettingsbuilders2.push_back(editViewSettingsBuilder2);
            workPart->SettingsManager()->ProcessForMultipleObjectsSettings(editsettingsbuilders2);

            editViewSettingsBuilder2->ViewCommonViewLabel()->SetLetter(nstr3);

            NXOpen::NXObject* nXObject2;
            nXObject2 = editViewSettingsBuilder2->Commit();
            editViewSettingsBuilder2->Destroy();
        }
        catch (std::exception& ex)
        {
            start = false;
            uc1601(const_cast<char*>("“акой символ уже используетс€"), 1);
        }

        //if (start)
        {
            //UF_ATTR_set_string_user_attribute(view_tag, "VWLETTER", UF_ATTR_NOT_ARRAY, (char*)nstr3.GetText(), &has_attribute);
            UF_ATTR_set_string_user_attribute(view_tag, "VWLETTER_DISP", UF_ATTR_NOT_ARRAY, string_value, &has_attribute);
            tag_t sxline_tag;
            UF_DRAW_ask_sxline_of_sxview(view_tag, &sxline_tag);

            //UF_UI_write_listing_window(("sxline - " + std::to_string(sxline_tag)).c_str());
            //UF_UI_write_listing_window("\n");

            double step_dir[3];
            double arrow_dir[3];
            tag_t pview_tag;
            int num_sxviews;
            tag_p_t sxview_tags;
            int num_sxsegs;
            tag_p_t sxseg_tags;
            UF_DRAW_sxline_status_t sxline_status;
            //UF_DRAW_ask_unfolded_sxline
                UF_DRAW_ask_simple_sxline(sxline_tag, step_dir, arrow_dir, &pview_tag, &num_sxviews, &sxview_tags, &num_sxsegs, &sxseg_tags, &sxline_status);

            char name[UF_OBJ_NAME_LEN + 1];
            //UF_OBJ_ask_name(pview_tag, name);
            //UF_UI_write_listing_window(("name - " + std::string(name)).c_str());
            //UF_UI_write_listing_window("\n");

            //UF_UI_write_listing_window(("pview_tag - " + std::to_string(pview_tag)).c_str());
            //UF_UI_write_listing_window("\n");

            tag_t im;
            UF_DRAW_ask_drawing_of_view(pview_tag, &im);
            UF_OBJ_ask_name(im, name);
            //UF_UI_write_listing_window(("im - " + std::string(name)).c_str());
            //UF_UI_write_listing_window("\n");

            std::string str_num_draw = name;
            auto slashPosition4 = str_num_draw.find_first_of(" "); // начина€ с 11 плюсов
            //strin4.erase(strin4.begin() + slashPosition4 + 1, strin4.end()); // с начала
            str_num_draw.erase(str_num_draw.begin(), str_num_draw.begin() + slashPosition4); //с конца
            //”дал€ем лишние пробелы из строки
            str_num_draw.erase(remove(str_num_draw.begin(), str_num_draw.end(), ' '), str_num_draw.end()); //с конца


            UF_free(sxview_tags);
            UF_free(sxseg_tags);


            std::vector<NXOpen::DisplayableObject*> viewlabels1;
            NXOpen::Annotations::Note* note3(dynamic_cast<NXOpen::Annotations::Note*>(NXObjectManager::Get(view_label_tag)));
            viewlabels1.push_back(note3);

            NXOpen::Drawings::EditViewLabelSettingsBuilder* editViewLabelSettingsBuilder1;
            editViewLabelSettingsBuilder1 = workPart->SettingsManager()->CreateDrawingEditViewLabelSettingsBuilder(viewlabels1);

            std::vector<NXOpen::Drafting::BaseEditSettingsBuilder*> editsettingsbuilders1;
            editsettingsbuilders1.push_back(editViewLabelSettingsBuilder1);
            workPart->SettingsManager()->ProcessForMultipleObjectsSettings(editsettingsbuilders1);

            editViewLabelSettingsBuilder1->ViewSectionLabel()->SetCustomizedViewLabel(true);

            std::vector<NXString> customizedtext1;
            NXOpen::Drawings::ViewSectionLabelBuilder* ViewSectionLabel(editViewLabelSettingsBuilder1->ViewSectionLabel());
            std::vector<NXString> GetCustomizedViewLabelText(ViewSectionLabel->GetCustomizedViewLabelText());


            //‘ормируем строку --> <C2.0000><W & Ref1 * 0@VWLETTER_DISP>-<W & Ref1 * 0@VWLETTER_DISP><C>
            std::string strin = (std::string)GetCustomizedViewLabelText[0].GetText();
            auto slashPosition = strin.find_first_of("-"); // начина€ с 11 плюсов
            //strin.erase(strin.begin() + slashPosition, strin.end()); // с начала
            strin.erase(strin.begin(), strin.begin() + slashPosition + 1); //с конца

            std::string strin1 = (std::string)GetCustomizedViewLabelText[0].GetText();
            auto slashPosition1 = strin1.find_first_of('@'); // начина€ с 11 плюсов
            strin1.erase(strin1.begin() + slashPosition1, strin1.end()); // с начала

            std::string strin2 = strin;
            auto slashPosition2 = strin2.find_first_of('@'); // начина€ с 11 плюсов
            strin2.erase(strin2.begin() + slashPosition2, strin2.end()); // с начала

           /*UF_UI_write_listing_window(strin1.c_str());
            UF_UI_write_listing_window("\n");
            UF_UI_write_listing_window(strin2.c_str());
            UF_UI_write_listing_window("\n");
            UF_UI_write_listing_window(strin.c_str());
            UF_UI_write_listing_window("\n");
            tag_t Solid_tag;
            

            UF_UI_write_listing_window("########################");
            UF_UI_write_listing_window("\n"); */   

            new_labelSTR_2 = (strin1 + "@VWLETTER>-" + strin2 + "@VWLETTER> (" + (std::string)str_num_draw + ")<C>");
            strcpy(string_label_value, new_labelSTR_2.c_str());
            customizedtext1.push_back(string_label_value);

            editViewLabelSettingsBuilder1->ViewSectionLabel()->SetCustomizedViewLabelText(customizedtext1);

            NXOpen::NXObject* nXObject1;
            nXObject1 = editViewLabelSettingsBuilder1->Commit();
            editViewLabelSettingsBuilder1->Destroy();
        }
    }
    catch (std::exception& ex)
    {
        UF_UI_open_listing_window();
        UF_UI_write_listing_window(ex.what() + '\n');
        //uc1601(const_cast<char*>("ќй, что то пошло не так..."), 1);
    }
}


void ufusr(char* param, int* retcod, int param_len)
{
    static unsigned char* c;
    c = { nxauthblock };

    int status = 0;
    status = UF_initialize();
    if (!status)
    {
        search_name();
    }
    UF_terminate();
}

int ufusr_ask_unload(void)
{
    return (UF_UNLOAD_IMMEDIATELY);
}
