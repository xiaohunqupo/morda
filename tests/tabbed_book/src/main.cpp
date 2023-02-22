#include <sstream>

#include <utki/debug.hpp>
#include <mordavokne/application.hpp>

#include "../../../src/morda/widgets/button/push_button.hpp"
#include "../../../src/morda/widgets/label/text.hpp"
#include "../../../src/morda/widgets/group/tabbed_book.hpp"

#include "sample_page.hpp"

utki::shared_ref<morda::tab> inflate_tab(morda::tabbed_book& tb, const std::string& name){
	auto t = tb.context.get().inflater.inflate_as<morda::tab>(R"(
		@tab{
			@row{
				@text{
					id{text}
					text{cube}
				}
				@push_button{
					id{activate_button}
					@text{
						text{A}
					}
				}
				@push_button{
					id{close_button}
					@image{
						layout{
							dx { 8dp }
							dy { 8dp }
						}
						image{morda_img_close}
					}
				}
			}
		}
	)");
	t.get().get_widget_as<morda::text>("text").set_text(name);

	auto& close_btn = t.get().get_widget_as<morda::push_button>("close_button");
	
	close_btn.click_handler = [
			tabbed_book_wp = utki::make_weak_from(tb),
			tab_wp = utki::make_weak(t)
		](morda::push_button& btn)
	{
		auto tb = tabbed_book_wp.lock();
		ASSERT(tb)

		auto t = tab_wp.lock();
		ASSERT(t)

		btn.context.get().run_from_ui_thread([tb, t]{
			tb->tear_out(*t);
		});
	};
	return t;
}

class application : public mordavokne::application{
public:
	application() :
			mordavokne::application(
					"morda-tests",
					[]() -> mordavokne::window_params {
						mordavokne::window_params wp(r4::vector2<unsigned>(640, 480));

						return wp;
					}()
				)
	{
		this->gui.initStandardWidgets(*this->get_res_file("../../res/morda_res/"));

		// this->gui.context->loader.mount_res_pack(*this->get_res_file("res/"));

		auto c = this->gui.context.get().inflater.inflate(
				*this->get_res_file("res/test.gui")
			);
		this->gui.set_root(c.to_shared_ptr());

		auto& book = c.get().get_widget_as<morda::tabbed_book>("book");

		auto& add_btn = c.get().get_widget_as<morda::push_button>("add_button");
		add_btn.click_handler = [bk = utki::make_shared_from(book), cnt = 0](morda::push_button& b) mutable {
			std::stringstream ss;
			ss << "page #" << cnt;
			auto txt = ss.str();
			++cnt;
			auto pg = utki::make_shared<sample_page>(b.context, txt);
			auto tb = inflate_tab(bk.get(), txt);

			tb.get().get_widget_as<morda::push_button>("activate_button").click_handler = [pgw = utki::make_weak(pg)](morda::push_button&){
				if(auto pg = pgw.lock()){
					pg->activate();
				}
			};

			bk.get().add(tb, pg);
		};
	}
};

mordavokne::application_factory app_fac([](auto args){
	return std::make_unique<::application>();
});
