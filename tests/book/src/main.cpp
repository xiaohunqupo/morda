#include <r4/quaternion.hpp>
#include <utki/debug.hpp>
#include <papki/fs_file.hpp>

#include <mordavokne/application.hpp>

#include "../../../src/morda/widgets/button/push_button.hpp"
#include "../../../src/morda/widgets/label/busy.hpp"
#include "../../../src/morda/widgets/group/book.hpp"
#include "../../../src/morda/widgets/button/tab_group.hpp"
#include "../../../src/morda/widgets/button/tab.hpp"
#include "../../../src/morda/widgets/label/text.hpp"

#include "pile_page.hpp"
#include "cube_page.hpp"

class application : public mordavokne::application{
public:
	application() :
			mordavokne::application("morda-tests", [](){
				mordavokne::window_params wp(r4::vector2<unsigned>(640, 480));

				return wp;
			}())
	{
		this->gui.init_standard_widgets(*this->get_res_file("../../res/morda_res/"));

		this->gui.context.get().loader.mount_res_pack(*this->get_res_file("res/"));

		auto c = this->gui.context.get().inflater.inflate(
				*this->get_res_file("res/test.gui")
			);
		this->gui.set_root(c);

		auto& book = c.get().get_widget_as<morda::book>("book");

		{
			auto mp = utki::make_shared<pile_page>(
					this->gui.context,
					treeml::read(R"qwertyuiop(
							@column{
								lp{dx{fill} dy{fill}}

								@push_button{
									id{cube_button}

									lp{
										dx{fill}
									}

									@text{
										text{Cube!}
									}
								}

								@push_button{
									id{stuff_button}

									lp{
										dx{fill}
									}

									@text{
										text{Stuff!}
									}
								}

								@push_button{
									id{close_button}

									lp{
										dx{fill}
									}

									@text{
										text{close}
									}
								}
							}
						)qwertyuiop")
				);
			mp.get().get_widget_as<morda::push_button>("cube_button").click_handler = [&mp = mp.get()](morda::push_button& b){
				mp.get_parent_book()->push(utki::make_shared<cube_page>(mp.context));
			};
			mp.get().get_widget_as<morda::push_button>("stuff_button").click_handler = [&mp = mp.get()](morda::push_button& b){
				auto pg = utki::make_shared<pile_page>(mp.context, treeml::read(R"qwertyuiop(
					@push_button{
						id{back_button}
						@text{
							text{"Go back"}
						}
					}
				)qwertyuiop"));
				pg.get().get_widget_as<morda::push_button>("back_button").click_handler = [&pg = pg.get()](morda::push_button& b){
					b.context.get().run_from_ui_thread([pg = utki::make_shared_from(pg)]{
						pg.get().tear_out();
					});
				};
				mp.get_parent_book()->push(pg);
			};
			mp.get().get_widget_as<morda::push_button>("close_button").click_handler = [&mp = mp.get()](morda::push_button& b){
				b.context.get().run_from_ui_thread([pg = utki::make_shared_from(mp)]{
					pg.get().tear_out();
				});
			};
			book.push(mp);
		}
	}
};

const mordavokne::application_factory app_fac([](auto args){
	return std::make_unique<::application>();
});
