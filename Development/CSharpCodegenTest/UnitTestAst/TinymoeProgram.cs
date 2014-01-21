using System;
using System.Collections.Generic;
using TinymoeDotNet;

namespace TinymoeProgramNamespace
{
	public class TinymoeProgram : TinymoeOperations
	{
		public readonly TinymoeObject standard_library__exiting_block = new TinymoeSymbol("standard_library__exiting_block");

		public class standard_library__continuation_trap : TinymoeObject
		{
			public standard_library__continuation_trap()
			{
				SetField("continuation", null);
				SetField("previous_trap", null);
			}
		}

		public class standard_library__continuation_fall_back_argument : TinymoeObject
		{
			public standard_library__continuation_fall_back_argument()
			{
				SetField("value", null);
				SetField("trap", null);
				SetField("fall_back_counter", null);
			}
		}

		public class standard_library__continuation_state : TinymoeObject
		{
			public standard_library__continuation_state()
			{
				SetField("flag", null);
				SetField("argument", null);
				SetField("continuation", null);
				SetField("trap", null);
			}
		}

		public class standard_library__continuation_coroutine : TinymoeObject
		{
			public standard_library__continuation_coroutine()
			{
				SetField("body", null);
				SetField("continuation", null);
				SetField("trap", null);
				SetField("fall_back_counter", null);
			}
		}

		public void standard_library__reset_continuation_state__expression_to__expression(TinymoeObject _state, TinymoeObject state, TinymoeObject flag, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			state.SetField("flag", flag);
			state.SetField("argument", null);
			state.SetField("continuation", null);
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__call__expression(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__trap__expression_internal(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_trap = null;
			the_current_trap = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {continuation, state.GetField("trap")}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			state.SetField("trap", the_current_trap);
			Invoke(value, new TinymoeObject[] {
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _result_3 = __args__[1];
					_the_result = _result_3;
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				});
		}

		public void standard_library__trap__expression(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression_internal(
				state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("trap", state.GetField("trap").GetField("previous_trap"));
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__fall_into_the_previous_trap(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject the_current_trap = null;
			the_current_trap = state.GetField("trap").GetField("continuation");
			TinymoeObject _state_0 = null;
			_state_0 = state;
			Invoke(the_current_trap, new TinymoeObject[] {
				_state_0,
				null
				});
		}

		public void standard_library__enable_fall_back_to__expression_with__expression_and__expression(TinymoeObject _state, TinymoeObject state, TinymoeObject continuation, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			state.SetField("continuation", continuation);
			state.SetField("argument", new standard_library__continuation_fall_back_argument().SetFields(new TinymoeObject[] {value, state.GetField("trap")}).FinishConstruction());
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__fall_back_counter_between__expression_and__expression_from__primitive(TinymoeObject _state, TinymoeObject top_trap, TinymoeObject current_trap, TinymoeObject counter, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = NE(top_trap, current_trap);
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(true)))).Value)
			{
				standard_library__fall_back_counter_between__expression_and__expression_from__primitive(
					_state,
					top_trap.GetField("previous_trap"),
					current_trap,
					Add(counter, new TinymoeInteger(1)),
					new TinymoeFunction(__args___x2 => 
					{
						TinymoeObject _state_4 = __args___x2[0];
						TinymoeObject _result_5 = __args___x2[1];
						_the_result = _result_5;
						Invoke(_select_continuation_2, new TinymoeObject[] {
							_state_4,
							_result_5
							});
					})
					);
			}
			else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(false)))).Value)
			{
				_the_result = counter;
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__set_fall_back_counter_of__expression(TinymoeObject _state, TinymoeObject state, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject top_trap = null;
			top_trap = state.GetField("argument").GetField("trap");
			TinymoeObject _state_0 = null;
			_state_0 = _state;
			TinymoeObject current_trap = null;
			current_trap = state.GetField("trap");
			TinymoeObject _state_2 = null;
			_state_2 = _state_0;
			standard_library__fall_back_counter_between__expression_and__expression_from__primitive(
				_state_2,
				top_trap,
				current_trap,
				Negative(new TinymoeInteger(1)),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_4 = __args__[0];
					TinymoeObject _result_5 = __args__[1];
					TinymoeObject counter = null;
					counter = _result_5;
					state.GetField("argument").SetField("fall_back_counter", counter);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__restored_trap__expression_with_fall_back__expression_and_counter__primitive(TinymoeObject _state, TinymoeObject trap, TinymoeObject fall_back_trap, TinymoeObject counter, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = counter;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeInteger(0)))).Value)
			{
				_the_result = trap;
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
			else
			{
				standard_library__restored_trap__expression_with_fall_back__expression_and_counter__primitive(
					_state,
					trap,
					fall_back_trap.GetField("previous_trap"),
					Sub(counter, new TinymoeInteger(1)),
					new TinymoeFunction(__args___x2 => 
					{
						TinymoeObject _state_4 = __args___x2[0];
						TinymoeObject _result_5 = __args___x2[1];
						TinymoeObject previous_trap = null;
						previous_trap = _result_5;
						_the_result = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {fall_back_trap.GetField("continuation"), previous_trap}).FinishConstruction();
						Invoke(_select_continuation_2, new TinymoeObject[] {
							_state_4,
							_result_5
							});
					})
					);
			}
		}

		public void standard_library__trap__expression_with_fall_back_enabled_internal(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_trap = null;
			the_current_trap = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {continuation, state.GetField("trap")}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			state.SetField("trap", the_current_trap);
			Invoke(value, new TinymoeObject[] {
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _result_3 = __args__[1];
					_the_result = _result_3;
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				});
		}

		public void standard_library__trap__expression_with_fall_back_enabled(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression_with_fall_back_enabled_internal(
				state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("trap", state.GetField("trap").GetField("previous_trap"));
					standard_library__set_fall_back_counter_of__expression(
						_state_0,
						state,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__trap__expression_with_fall_back__expression_restored_internal(TinymoeObject state, TinymoeObject value, TinymoeObject fall_back, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_trap = null;
			the_current_trap = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {continuation, state.GetField("trap")}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			standard_library__restored_trap__expression_with_fall_back__expression_and_counter__primitive(
				_state_0,
				the_current_trap,
				fall_back.GetField("trap"),
				fall_back.GetField("fall_back_counter"),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _result_3 = __args__[1];
					TinymoeObject new_trap = null;
					new_trap = _result_3;
					state.SetField("trap", new_trap);
					Invoke(value, new TinymoeObject[] {
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _result_5 = __args___x2[1];
							_the_result = _result_5;
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__trap__expression_with_fall_back__expression_restored(TinymoeObject state, TinymoeObject value, TinymoeObject fall_back, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression_with_fall_back__expression_restored_internal(
				state,
				value,
				fall_back,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("trap", state.GetField("trap").GetField("previous_trap"));
					standard_library__set_fall_back_counter_of__expression(
						_state_0,
						state,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__create_closure_to_assignable(TinymoeObject _state, TinymoeObject body, TinymoeObject _read_closure, TinymoeObject _write_closure, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_write_closure, new TinymoeObject[] {
				_state,
				body,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void standard_library__new_coroutine_from__primitive(TinymoeObject _state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = new standard_library__continuation_coroutine().SetFields(new TinymoeObject[] {}).FinishConstruction();
			_the_result.SetField("body", body);
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__coroutine__expression_stopped(TinymoeObject _state, TinymoeObject coroutine, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = And(EQ(coroutine.GetField("body"), null), EQ(coroutine.GetField("continuation"), null));
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__run_coroutine__expression(TinymoeObject state, TinymoeObject coroutine, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject need_to_exit = null;
			need_to_exit = new TinymoeBoolean(false);
			TinymoeObject _state_0 = null;
			_state_0 = state;
			standard_library__if__expression(
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_4 = __args__[0];
					TinymoeObject _continuation_5 = __args__[1];
					standard_library__trap__expression_with_fall_back_enabled(
						_state_4,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_6 = __args___x2[0];
							TinymoeObject _continuation_7 = __args___x2[1];
							Invoke(coroutine.GetField("body"), new TinymoeObject[] {
								_state_6,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_8 = __args___x3[0];
									TinymoeObject _result_9 = __args___x3[1];
									Invoke(_continuation_7, new TinymoeObject[] {
										_state_6,
										_result_9
										});
								})
								});
						}),
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_10 = __args___x4[0];
							TinymoeObject _result_11 = __args___x4[1];
							coroutine.SetField("body", null);
							Invoke(_continuation_5, new TinymoeObject[] {
								_state_10,
								_result_11
								});
						})
						);
				}),
				NE(coroutine.GetField("body"), null),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_2 = __args___x5[0];
					TinymoeObject _result_3 = __args___x5[1];
					standard_library__else_if__expression(
						_state_2,
						_result_3,
						new TinymoeFunction(__args___x6 => 
						{
							TinymoeObject _state_16 = __args___x6[0];
							TinymoeObject _continuation_17 = __args___x6[1];
							standard_library__trap__expression_with_fall_back__expression_restored(
								_state_16,
								new TinymoeFunction(__args___x7 => 
								{
									TinymoeObject _state_18 = __args___x7[0];
									TinymoeObject _continuation_19 = __args___x7[1];
									Invoke(coroutine.GetField("continuation"), new TinymoeObject[] {
										_state_18,
										null
										});
								}),
								coroutine,
								_continuation_17
								);
						}),
						new TinymoeFunction(__args___x8 => 
						{
							TinymoeObject _state_12 = __args___x8[0];
							TinymoeObject _continuation_13 = __args___x8[1];
							Invoke(_continuation_13, new TinymoeObject[] {
								_state_12,
								NE(coroutine.GetField("continuation"), null)
								});
						}),
						new TinymoeFunction(__args___x9 => 
						{
							TinymoeObject _state_14 = __args___x9[0];
							TinymoeObject _result_15 = __args___x9[1];
							standard_library__else(
								_state_14,
								_result_15,
								new TinymoeFunction(__args___x10 => 
								{
									TinymoeObject _state_26 = __args___x10[0];
									TinymoeObject _continuation_27 = __args___x10[1];
									need_to_exit = new TinymoeBoolean(true);
									Invoke(_continuation_27, new TinymoeObject[] {
										_state_26,
										null
										});
								}),
								new TinymoeFunction(__args___x11 => 
								{
									TinymoeObject _state_24 = __args___x11[0];
									TinymoeObject _result_25 = __args___x11[1];
									standard_library__if__expression(
										_state_24,
										new TinymoeFunction(__args___x12 => 
										{
											TinymoeObject _state_30 = __args___x12[0];
											TinymoeObject _continuation_31 = __args___x12[1];
											coroutine.SetField("trap", state.GetField("argument").GetField("trap"));
											coroutine.SetField("fall_back_counter", state.GetField("argument").GetField("fall_back_counter"));
											Invoke(_continuation_31, new TinymoeObject[] {
												_state_30,
												null
												});
										}),
										And(Not(need_to_exit), new TinymoeBoolean(state.GetField("argument") is standard_library__continuation_fall_back_argument)),
										new TinymoeFunction(__args___x13 => 
										{
											TinymoeObject _state_28 = __args___x13[0];
											TinymoeObject _result_29 = __args___x13[1];
											Invoke(continuation, new TinymoeObject[] {
												state,
												_the_result
												});
										})
										);
								})
								);
						})
						);
				})
				);
		}

		public void standard_library__continue_coroutine__expression(TinymoeObject state, TinymoeObject coroutine, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			coroutine.SetField("continuation", state.GetField("continuation"));
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				null,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__stop_coroutine__expression(TinymoeObject state, TinymoeObject coroutine, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			coroutine.SetField("continuation", null);
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				null,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__pause_coroutine_to__expression_with__expression_and__expression(TinymoeObject state, TinymoeObject flag, TinymoeObject coroutine_continuation, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				flag,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__enable_fall_back_to__expression_with__expression_and__expression(
						_state_0,
						state,
						coroutine_continuation,
						value,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							standard_library__fall_into_the_previous_trap(
								_state_2,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_4 = __args___x3[0];
									TinymoeObject _result_5 = __args___x3[1];
									Invoke(continuation, new TinymoeObject[] {
										state,
										_the_result
										});
								})
								);
						})
						);
				})
				);
		}

		public void standard_library__break(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__breaking_repeating,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__continue(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__continuing_repeating,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__repeat(TinymoeObject state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _continuation_1 = __args__[1];
					Invoke(body, new TinymoeObject[] {
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation_1, new TinymoeObject[] {
								_state_0,
								_result_3
								});
						})
						});
				}),
				new TinymoeFunction(__args___x3 => 
				{
					TinymoeObject _state_4 = __args___x3[0];
					TinymoeObject _result_5 = __args___x3[1];
					TinymoeObject _select_continuation_8 = null;
					TinymoeObject _select_value_9 = null;
					_select_continuation_8 = new TinymoeFunction(__args___x4 => 
					{
						TinymoeObject _state_6 = __args___x4[0];
						TinymoeObject _result_7 = __args___x4[1];
						Invoke(_continuation, new TinymoeObject[] {
							state,
							_the_result
							});
					});
					_select_value_9 = state.GetField("flag");
					if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, standard_library__breaking_repeating))).Value)
					{
						standard_library__reset_continuation_state__expression_to__expression(
							_state_4,
							state,
							null,
							_select_continuation_8
							);
					}
					else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, standard_library__continuing_repeating))).Value)
					{
						standard_library__reset_continuation_state__expression_to__expression(
							_state_4,
							state,
							null,
							new TinymoeFunction(__args___x5 => 
							{
								TinymoeObject _state_12 = __args___x5[0];
								TinymoeObject _result_13 = __args___x5[1];
								standard_library__repeat(
									_state_12,
									body,
									new TinymoeFunction(__args___x6 => 
									{
										TinymoeObject _state_14 = __args___x6[0];
										TinymoeObject _result_15 = __args___x6[1];
										standard_library__call__expression(
											_state_14,
											_result_15,
											_select_continuation_8
											);
									})
									);
							})
							);
					}
					else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, null))).Value)
					{
						standard_library__repeat(
							_state_4,
							body,
							new TinymoeFunction(__args___x7 => 
							{
								TinymoeObject _state_18 = __args___x7[0];
								TinymoeObject _result_19 = __args___x7[1];
								standard_library__call__expression(
									_state_18,
									_result_19,
									_select_continuation_8
									);
							})
							);
					}
					else
					{
						standard_library__fall_into_the_previous_trap(
							_state_4,
							_select_continuation_8
							);
					}
				})
				);
		}

		public void standard_library__repeat_while__expression(TinymoeObject _state, TinymoeObject body, TinymoeObject condition, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__repeat(
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					Invoke(condition, new TinymoeObject[] {
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _result_5 = __args___x2[1];
							standard_library__if__expression(
								_state_4,
								new TinymoeFunction(__args___x3 => standard_library__break(__args___x3[0], __args___x3[1])),
								Not(_result_5),
								new TinymoeFunction(__args___x4 => 
								{
									TinymoeObject _state_6 = __args___x4[0];
									TinymoeObject _result_7 = __args___x4[1];
									Invoke(body, new TinymoeObject[] {
										_state_6,
										_continuation_3
										});
								})
								);
						})
						});
				}),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_0 = __args___x5[0];
					TinymoeObject _result_1 = __args___x5[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__repeat_with__argument_from__expression_to__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject lower_bound, TinymoeObject upper_bound, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_number = null;
			the_current_number = Sub(lower_bound, new TinymoeInteger(1));
			TinymoeObject _state_0 = null;
			_state_0 = _state;
			standard_library__repeat_while__expression(
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_6 = __args__[0];
					TinymoeObject _continuation_7 = __args__[1];
					standard_library__add__expression_to_assignable(
						_state_6,
						new TinymoeInteger(1),
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_8 = __args___x2[0];
							TinymoeObject _continuation_9 = __args___x2[1];
							Invoke(_continuation_9, new TinymoeObject[] {
								_state_8,
								the_current_number
								});
						}),
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_10 = __args___x3[0];
							TinymoeObject _input_11 = __args___x3[1];
							TinymoeObject _continuation_12 = __args___x3[2];
							the_current_number = _input_11;
							Invoke(_continuation_12, new TinymoeObject[] {
								_state_10,
								null
								});
						}),
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_13 = __args___x4[0];
							TinymoeObject _result_14 = __args___x4[1];
							Invoke(deal_with__expression, new TinymoeObject[] {
								_state_13,
								the_current_number,
								_continuation_7
								});
						})
						);
				}),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_2 = __args___x5[0];
					TinymoeObject _continuation_3 = __args___x5[1];
					Invoke(_continuation_3, new TinymoeObject[] {
						_state_2,
						LT(the_current_number, upper_bound)
						});
				}),
				new TinymoeFunction(__args___x6 => 
				{
					TinymoeObject _state_4 = __args___x6[0];
					TinymoeObject _result_5 = __args___x6[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__repeat_with__argument_in__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			Invoke(items.GetField("$dispatch<>standard_library::repeat_with_$argument_in_$expression"), new TinymoeObject[] {
				_state,
				deal_with__expression,
				items,
				_continuation
				});
		}

		public void standard_library__repeat_with__argument_in__expression_array_(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__repeat_with__argument_from__expression_to__expression(
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject index = __args__[1];
					TinymoeObject _continuation_3 = __args__[2];
					Invoke(deal_with__expression, new TinymoeObject[] {
						_state_2,
						ArrayGet(items, index),
						_continuation_3
						});
				}),
				new TinymoeInteger(1),
				ArrayLength(items),
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__if__expression(TinymoeObject _state, TinymoeObject body, TinymoeObject condition, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = condition;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = _the_result;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(true)))).Value)
			{
				Invoke(body, new TinymoeObject[] {
					_state,
					_select_continuation_2
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__else_if__expression(TinymoeObject _state, TinymoeObject signal, TinymoeObject body, TinymoeObject condition, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = signal;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = _the_result;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(false)))).Value)
			{
				Invoke(condition, new TinymoeObject[] {
					_state,
					new TinymoeFunction(__args___x2 => 
					{
						TinymoeObject _state_4 = __args___x2[0];
						TinymoeObject _result_5 = __args___x2[1];
						_the_result = _result_5;
						TinymoeObject _select_continuation_8 = null;
						TinymoeObject _select_value_9 = null;
						_select_continuation_8 = _select_continuation_2;
						_select_value_9 = _the_result;
						if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, new TinymoeBoolean(true)))).Value)
						{
							Invoke(body, new TinymoeObject[] {
								_state_4,
								_select_continuation_8
								});
						}
						else
						{
							Invoke(_select_continuation_8, new TinymoeObject[] {
								_state_4,
								null
								});
						}
					})
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__else(TinymoeObject _state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = signal;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = _the_result;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(false)))).Value)
			{
				Invoke(body, new TinymoeObject[] {
					_state,
					_select_continuation_2
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__raise__expression(TinymoeObject state, TinymoeObject exception, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__raising_exception,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("argument", exception);
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__try(TinymoeObject state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _continuation_1 = __args__[1];
					Invoke(body, new TinymoeObject[] {
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation_1, new TinymoeObject[] {
								_state_0,
								_result_3
								});
						})
						});
				}),
				new TinymoeFunction(__args___x3 => 
				{
					TinymoeObject _state_4 = __args___x3[0];
					TinymoeObject _result_5 = __args___x3[1];
					TinymoeObject _select_continuation_8 = null;
					TinymoeObject _select_value_9 = null;
					_select_continuation_8 = new TinymoeFunction(__args___x4 => 
					{
						TinymoeObject _state_6 = __args___x4[0];
						TinymoeObject _result_7 = __args___x4[1];
						Invoke(_continuation, new TinymoeObject[] {
							state,
							_the_result
							});
					});
					_select_value_9 = state.GetField("flag");
					if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, standard_library__raising_exception))).Value)
					{
						_the_result = state.GetField("argument");
						standard_library__reset_continuation_state__expression_to__expression(
							_state_4,
							state,
							null,
							_select_continuation_8
							);
					}
					else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, null))).Value)
					{
						Invoke(_select_continuation_8, new TinymoeObject[] {
							_state_4,
							null
							});
					}
					else
					{
						standard_library__fall_into_the_previous_trap(
							_state_4,
							_select_continuation_8
							);
					}
				})
				);
		}

		public void standard_library__else_try(TinymoeObject state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__if__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					standard_library__trap__expression(
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _continuation_5 = __args___x2[1];
							Invoke(body, new TinymoeObject[] {
								_state_4,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_6 = __args___x3[0];
									TinymoeObject _result_7 = __args___x3[1];
									Invoke(_continuation_5, new TinymoeObject[] {
										_state_4,
										_result_7
										});
								})
								});
						}),
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_8 = __args___x4[0];
							TinymoeObject _result_9 = __args___x4[1];
							TinymoeObject _select_continuation_12 = null;
							TinymoeObject _select_value_13 = null;
							_select_continuation_12 = _continuation_3;
							_select_value_13 = state.GetField("flag");
							if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_13, standard_library__raising_exception))).Value)
							{
								_the_result = state.GetField("argument");
								standard_library__reset_continuation_state__expression_to__expression(
									_state_8,
									state,
									null,
									_select_continuation_12
									);
							}
							else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_13, null))).Value)
							{
								Invoke(_select_continuation_12, new TinymoeObject[] {
									_state_8,
									null
									});
							}
							else
							{
								standard_library__fall_into_the_previous_trap(
									_state_8,
									_select_continuation_12
									);
							}
						})
						);
				}),
				NE(signal, null),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_0 = __args___x5[0];
					TinymoeObject _result_1 = __args___x5[1];
					Invoke(_continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__catch__argument(TinymoeObject state, TinymoeObject signal, TinymoeObject deal_with__expression, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__if__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					Invoke(deal_with__expression, new TinymoeObject[] {
						_state_2,
						signal,
						_continuation_3
						});
				}),
				NE(signal, null),
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__finally(TinymoeObject state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(body, new TinymoeObject[] {
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				});
		}

		public void standard_library__exit_program(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__exiting_program,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__exit_block__expression(TinymoeObject state, TinymoeObject handle, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__exiting_block,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("argument", handle);
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__named_block__argument(TinymoeObject state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject handle = null;
			handle = new TinymoeObject().SetFields(new TinymoeObject[] {}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			standard_library__trap__expression(
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					Invoke(body, new TinymoeObject[] {
						_state_2,
						handle,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _result_5 = __args___x2[1];
							Invoke(_continuation_3, new TinymoeObject[] {
								_state_2,
								_result_5
								});
						})
						});
				}),
				new TinymoeFunction(__args___x3 => 
				{
					TinymoeObject _state_6 = __args___x3[0];
					TinymoeObject _result_7 = __args___x3[1];
					standard_library__if__expression(
						_state_6,
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_10 = __args___x4[0];
							TinymoeObject _continuation_11 = __args___x4[1];
							standard_library__reset_continuation_state__expression_to__expression(
								_state_10,
								state,
								null,
								_continuation_11
								);
						}),
						And(EQ(state.GetField("flag"), standard_library__exiting_block), EQ(state.GetField("argument"), handle)),
						new TinymoeFunction(__args___x5 => 
						{
							TinymoeObject _state_8 = __args___x5[0];
							TinymoeObject _result_9 = __args___x5[1];
							standard_library__else_if__expression(
								_state_8,
								_result_9,
								new TinymoeFunction(__args___x6 => standard_library__fall_into_the_previous_trap(__args___x6[0], __args___x6[1])),
								new TinymoeFunction(__args___x7 => 
								{
									TinymoeObject _state_14 = __args___x7[0];
									TinymoeObject _continuation_15 = __args___x7[1];
									Invoke(_continuation_15, new TinymoeObject[] {
										_state_14,
										NE(state.GetField("flag"), null)
										});
								}),
								new TinymoeFunction(__args___x8 => 
								{
									TinymoeObject _state_16 = __args___x8[0];
									TinymoeObject _result_17 = __args___x8[1];
									Invoke(_continuation, new TinymoeObject[] {
										state,
										_the_result
										});
								})
								);
						})
						);
				})
				);
		}

		public void standard_library__add__expression_to_assignable(TinymoeObject _state, TinymoeObject value, TinymoeObject _read_variable, TinymoeObject _write_variable, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_read_variable, new TinymoeObject[] {
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_write_variable, new TinymoeObject[] {
						_state_0,
						Add(_result_1, value),
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				});
		}

		public void standard_library__substract__expression_from_assignable(TinymoeObject _state, TinymoeObject value, TinymoeObject _read_variable, TinymoeObject _write_variable, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_read_variable, new TinymoeObject[] {
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_write_variable, new TinymoeObject[] {
						_state_0,
						Sub(_result_1, value),
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				});
		}

		public void standard_library__boolean_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToBoolean")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void standard_library__integer_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToInteger")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void standard_library__floating_point_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToFloat")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void standard_library__string_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToString")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public readonly TinymoeObject standard_library__breaking_repeating = new TinymoeSymbol("standard_library__breaking_repeating");

		public readonly TinymoeObject standard_library__continuing_repeating = new TinymoeSymbol("standard_library__continuing_repeating");

		public readonly TinymoeObject standard_library__raising_exception = new TinymoeSymbol("standard_library__raising_exception");

		public readonly TinymoeObject standard_library__exiting_program = new TinymoeSymbol("standard_library__exiting_program");

		public void unit_test__print__expression(TinymoeObject _state, TinymoeObject message, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("Print")), new TinymoeObject[] {
				_state,
				message,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void unit_test__test_case__expression(TinymoeObject _state, TinymoeObject run_the_test_case, TinymoeObject name, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__try(
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					Invoke(run_the_test_case, new TinymoeObject[] {
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _result_5 = __args___x2[1];
							unit_test__print__expression(
								_state_4,
								Concat(new TinymoeString("PASS: "), name),
								_continuation_3
								);
						})
						});
				}),
				new TinymoeFunction(__args___x3 => 
				{
					TinymoeObject _state_0 = __args___x3[0];
					TinymoeObject _result_1 = __args___x3[1];
					standard_library__catch__argument(
						_state_0,
						_result_1,
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_10 = __args___x4[0];
							TinymoeObject exception = __args___x4[1];
							TinymoeObject _continuation_11 = __args___x4[2];
							TinymoeObject _select_continuation_14 = null;
							TinymoeObject _select_value_15 = null;
							_select_continuation_14 = _continuation_11;
							_select_value_15 = exception;
							if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_15, new TinymoeString("AssertionFailure")))).Value)
							{
								unit_test__print__expression(
									_state_10,
									Concat(new TinymoeString("FAIL: "), name),
									_select_continuation_14
									);
							}
							else
							{
								unit_test__print__expression(
									_state_10,
									Concat(new TinymoeString("HALT: "), name),
									_select_continuation_14
									);
							}
						}),
						new TinymoeFunction(__args___x5 => 
						{
							TinymoeObject _state_8 = __args___x5[0];
							TinymoeObject _result_9 = __args___x5[1];
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void unit_test__assert__expression_should_be__expression(TinymoeObject _state, TinymoeObject actual_value, TinymoeObject expected_value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__if__expression(
				_state,
				new TinymoeFunction(__args__ => unit_test__assert_fail(__args__[0], __args__[1])),
				NE(actual_value, expected_value),
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void unit_test__assert_fail(TinymoeObject _state, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				new TinymoeString("AssertionFailure"),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void unit_test__main(TinymoeObject _state, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			unit_test__test_case__expression(
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					unit_test__assert__expression_should_be__expression(
						_state_2,
						Add(new TinymoeInteger(1), new TinymoeInteger(1)),
						new TinymoeInteger(2),
						_continuation_3
						);
				}),
				new TinymoeString("1+1=2"),
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					unit_test__test_case__expression(
						_state_0,
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_8 = __args___x3[0];
							TinymoeObject _continuation_9 = __args___x3[1];
							TinymoeObject sum = null;
							sum = new TinymoeInteger(0);
							TinymoeObject _state_10 = null;
							_state_10 = _state_8;
							standard_library__repeat_with__argument_from__expression_to__expression(
								_state_10,
								new TinymoeFunction(__args___x4 => 
								{
									TinymoeObject _state_14 = __args___x4[0];
									TinymoeObject i = __args___x4[1];
									TinymoeObject _continuation_15 = __args___x4[2];
									standard_library__if__expression(
										_state_14,
										new TinymoeFunction(__args___x5 => standard_library__break(__args___x5[0], __args___x5[1])),
										GT(i, new TinymoeInteger(10)),
										new TinymoeFunction(__args___x6 => 
										{
											TinymoeObject _state_16 = __args___x6[0];
											TinymoeObject _result_17 = __args___x6[1];
											standard_library__add__expression_to_assignable(
												_state_16,
												i,
												new TinymoeFunction(__args___x7 => 
												{
													TinymoeObject _state_22 = __args___x7[0];
													TinymoeObject _continuation_23 = __args___x7[1];
													Invoke(_continuation_23, new TinymoeObject[] {
														_state_22,
														sum
														});
												}),
												new TinymoeFunction(__args___x8 => 
												{
													TinymoeObject _state_24 = __args___x8[0];
													TinymoeObject _input_25 = __args___x8[1];
													TinymoeObject _continuation_26 = __args___x8[2];
													sum = _input_25;
													Invoke(_continuation_26, new TinymoeObject[] {
														_state_24,
														null
														});
												}),
												_continuation_15
												);
										})
										);
								}),
								new TinymoeInteger(1),
								new TinymoeInteger(100),
								new TinymoeFunction(__args___x9 => 
								{
									TinymoeObject _state_12 = __args___x9[0];
									TinymoeObject _result_13 = __args___x9[1];
									unit_test__assert__expression_should_be__expression(
										_state_12,
										sum,
										new TinymoeInteger(55),
										_continuation_9
										);
								})
								);
						}),
						new TinymoeString("Break should stop the repeating (1)"),
						new TinymoeFunction(__args___x10 => 
						{
							TinymoeObject _state_6 = __args___x10[0];
							TinymoeObject _result_7 = __args___x10[1];
							unit_test__test_case__expression(
								_state_6,
								new TinymoeFunction(__args___x11 => 
								{
									TinymoeObject _state_33 = __args___x11[0];
									TinymoeObject _continuation_34 = __args___x11[1];
									TinymoeObject sum_x2 = null;
									sum_x2 = new TinymoeInteger(0);
									TinymoeObject _state_35 = null;
									_state_35 = _state_33;
									standard_library__repeat_with__argument_from__expression_to__expression(
										_state_35,
										new TinymoeFunction(__args___x12 => 
										{
											TinymoeObject _state_39 = __args___x12[0];
											TinymoeObject i_x2 = __args___x12[1];
											TinymoeObject _continuation_40 = __args___x12[2];
											standard_library__add__expression_to_assignable(
												_state_39,
												i_x2,
												new TinymoeFunction(__args___x13 => 
												{
													TinymoeObject _state_41 = __args___x13[0];
													TinymoeObject _continuation_42 = __args___x13[1];
													Invoke(_continuation_42, new TinymoeObject[] {
														_state_41,
														sum_x2
														});
												}),
												new TinymoeFunction(__args___x14 => 
												{
													TinymoeObject _state_43 = __args___x14[0];
													TinymoeObject _input_44 = __args___x14[1];
													TinymoeObject _continuation_45 = __args___x14[2];
													sum_x2 = _input_44;
													Invoke(_continuation_45, new TinymoeObject[] {
														_state_43,
														null
														});
												}),
												new TinymoeFunction(__args___x15 => 
												{
													TinymoeObject _state_46 = __args___x15[0];
													TinymoeObject _result_47 = __args___x15[1];
													standard_library__if__expression(
														_state_46,
														new TinymoeFunction(__args___x16 => standard_library__break(__args___x16[0], __args___x16[1])),
														GE(i_x2, new TinymoeInteger(10)),
														_continuation_40
														);
												})
												);
										}),
										new TinymoeInteger(1),
										new TinymoeInteger(100),
										new TinymoeFunction(__args___x17 => 
										{
											TinymoeObject _state_37 = __args___x17[0];
											TinymoeObject _result_38 = __args___x17[1];
											unit_test__assert__expression_should_be__expression(
												_state_37,
												sum_x2,
												new TinymoeInteger(55),
												_continuation_34
												);
										})
										);
								}),
								new TinymoeString("Break should stop the repeating (2)"),
								new TinymoeFunction(__args___x18 => 
								{
									TinymoeObject _state_31 = __args___x18[0];
									TinymoeObject _result_32 = __args___x18[1];
									unit_test__test_case__expression(
										_state_31,
										new TinymoeFunction(__args___x19 => 
										{
											TinymoeObject _state_58 = __args___x19[0];
											TinymoeObject _continuation_59 = __args___x19[1];
											TinymoeObject sum_x3 = null;
											sum_x3 = new TinymoeInteger(0);
											TinymoeObject _state_60 = null;
											_state_60 = _state_58;
											standard_library__repeat_with__argument_from__expression_to__expression(
												_state_60,
												new TinymoeFunction(__args___x20 => 
												{
													TinymoeObject _state_64 = __args___x20[0];
													TinymoeObject i_x3 = __args___x20[1];
													TinymoeObject _continuation_65 = __args___x20[2];
													standard_library__if__expression(
														_state_64,
														new TinymoeFunction(__args___x21 => standard_library__continue(__args___x21[0], __args___x21[1])),
														GT(i_x3, new TinymoeInteger(10)),
														new TinymoeFunction(__args___x22 => 
														{
															TinymoeObject _state_66 = __args___x22[0];
															TinymoeObject _result_67 = __args___x22[1];
															standard_library__add__expression_to_assignable(
																_state_66,
																i_x3,
																new TinymoeFunction(__args___x23 => 
																{
																	TinymoeObject _state_72 = __args___x23[0];
																	TinymoeObject _continuation_73 = __args___x23[1];
																	Invoke(_continuation_73, new TinymoeObject[] {
																		_state_72,
																		sum_x3
																		});
																}),
																new TinymoeFunction(__args___x24 => 
																{
																	TinymoeObject _state_74 = __args___x24[0];
																	TinymoeObject _input_75 = __args___x24[1];
																	TinymoeObject _continuation_76 = __args___x24[2];
																	sum_x3 = _input_75;
																	Invoke(_continuation_76, new TinymoeObject[] {
																		_state_74,
																		null
																		});
																}),
																_continuation_65
																);
														})
														);
												}),
												new TinymoeInteger(1),
												new TinymoeInteger(100),
												new TinymoeFunction(__args___x25 => 
												{
													TinymoeObject _state_62 = __args___x25[0];
													TinymoeObject _result_63 = __args___x25[1];
													unit_test__assert__expression_should_be__expression(
														_state_62,
														sum_x3,
														new TinymoeInteger(55),
														_continuation_59
														);
												})
												);
										}),
										new TinymoeString("Continue should restart the next repeating"),
										new TinymoeFunction(__args___x26 => 
										{
											TinymoeObject _state_56 = __args___x26[0];
											TinymoeObject _result_57 = __args___x26[1];
											unit_test__test_case__expression(
												_state_56,
												new TinymoeFunction(__args___x27 => 
												{
													TinymoeObject _state_83 = __args___x27[0];
													TinymoeObject _continuation_84 = __args___x27[1];
													TinymoeObject a = null;
													a = new TinymoeInteger(0);
													TinymoeObject _state_85 = null;
													_state_85 = _state_83;
													TinymoeObject b = null;
													b = new TinymoeInteger(0);
													TinymoeObject _state_87 = null;
													_state_87 = _state_85;
													TinymoeObject c = null;
													c = new TinymoeInteger(0);
													TinymoeObject _state_89 = null;
													_state_89 = _state_87;
													standard_library__if__expression(
														_state_89,
														new TinymoeFunction(__args___x28 => 
														{
															TinymoeObject _state_93 = __args___x28[0];
															TinymoeObject _continuation_94 = __args___x28[1];
															a = new TinymoeInteger(1);
															Invoke(_continuation_94, new TinymoeObject[] {
																_state_93,
																null
																});
														}),
														new TinymoeBoolean(true),
														new TinymoeFunction(__args___x29 => 
														{
															TinymoeObject _state_91 = __args___x29[0];
															TinymoeObject _result_92 = __args___x29[1];
															standard_library__else_if__expression(
																_state_91,
																_result_92,
																new TinymoeFunction(__args___x30 => 
																{
																	TinymoeObject _state_99 = __args___x30[0];
																	TinymoeObject _continuation_100 = __args___x30[1];
																	a = new TinymoeInteger(2);
																	Invoke(_continuation_100, new TinymoeObject[] {
																		_state_99,
																		null
																		});
																}),
																new TinymoeFunction(__args___x31 => 
																{
																	TinymoeObject _state_95 = __args___x31[0];
																	TinymoeObject _continuation_96 = __args___x31[1];
																	Invoke(_continuation_96, new TinymoeObject[] {
																		_state_95,
																		new TinymoeBoolean(true)
																		});
																}),
																new TinymoeFunction(__args___x32 => 
																{
																	TinymoeObject _state_97 = __args___x32[0];
																	TinymoeObject _result_98 = __args___x32[1];
																	standard_library__else(
																		_state_97,
																		_result_98,
																		new TinymoeFunction(__args___x33 => 
																		{
																			TinymoeObject _state_103 = __args___x33[0];
																			TinymoeObject _continuation_104 = __args___x33[1];
																			a = new TinymoeInteger(3);
																			Invoke(_continuation_104, new TinymoeObject[] {
																				_state_103,
																				null
																				});
																		}),
																		new TinymoeFunction(__args___x34 => 
																		{
																			TinymoeObject _state_101 = __args___x34[0];
																			TinymoeObject _result_102 = __args___x34[1];
																			standard_library__if__expression(
																				_state_101,
																				new TinymoeFunction(__args___x35 => 
																				{
																					TinymoeObject _state_107 = __args___x35[0];
																					TinymoeObject _continuation_108 = __args___x35[1];
																					b = new TinymoeInteger(1);
																					Invoke(_continuation_108, new TinymoeObject[] {
																						_state_107,
																						null
																						});
																				}),
																				new TinymoeBoolean(false),
																				new TinymoeFunction(__args___x36 => 
																				{
																					TinymoeObject _state_105 = __args___x36[0];
																					TinymoeObject _result_106 = __args___x36[1];
																					standard_library__else_if__expression(
																						_state_105,
																						_result_106,
																						new TinymoeFunction(__args___x37 => 
																						{
																							TinymoeObject _state_113 = __args___x37[0];
																							TinymoeObject _continuation_114 = __args___x37[1];
																							b = new TinymoeInteger(2);
																							Invoke(_continuation_114, new TinymoeObject[] {
																								_state_113,
																								null
																								});
																						}),
																						new TinymoeFunction(__args___x38 => 
																						{
																							TinymoeObject _state_109 = __args___x38[0];
																							TinymoeObject _continuation_110 = __args___x38[1];
																							Invoke(_continuation_110, new TinymoeObject[] {
																								_state_109,
																								new TinymoeBoolean(true)
																								});
																						}),
																						new TinymoeFunction(__args___x39 => 
																						{
																							TinymoeObject _state_111 = __args___x39[0];
																							TinymoeObject _result_112 = __args___x39[1];
																							standard_library__else(
																								_state_111,
																								_result_112,
																								new TinymoeFunction(__args___x40 => 
																								{
																									TinymoeObject _state_117 = __args___x40[0];
																									TinymoeObject _continuation_118 = __args___x40[1];
																									b = new TinymoeInteger(3);
																									Invoke(_continuation_118, new TinymoeObject[] {
																										_state_117,
																										null
																										});
																								}),
																								new TinymoeFunction(__args___x41 => 
																								{
																									TinymoeObject _state_115 = __args___x41[0];
																									TinymoeObject _result_116 = __args___x41[1];
																									standard_library__if__expression(
																										_state_115,
																										new TinymoeFunction(__args___x42 => 
																										{
																											TinymoeObject _state_121 = __args___x42[0];
																											TinymoeObject _continuation_122 = __args___x42[1];
																											c = new TinymoeInteger(1);
																											Invoke(_continuation_122, new TinymoeObject[] {
																												_state_121,
																												null
																												});
																										}),
																										new TinymoeBoolean(false),
																										new TinymoeFunction(__args___x43 => 
																										{
																											TinymoeObject _state_119 = __args___x43[0];
																											TinymoeObject _result_120 = __args___x43[1];
																											standard_library__else_if__expression(
																												_state_119,
																												_result_120,
																												new TinymoeFunction(__args___x44 => 
																												{
																													TinymoeObject _state_127 = __args___x44[0];
																													TinymoeObject _continuation_128 = __args___x44[1];
																													c = new TinymoeInteger(2);
																													Invoke(_continuation_128, new TinymoeObject[] {
																														_state_127,
																														null
																														});
																												}),
																												new TinymoeFunction(__args___x45 => 
																												{
																													TinymoeObject _state_123 = __args___x45[0];
																													TinymoeObject _continuation_124 = __args___x45[1];
																													Invoke(_continuation_124, new TinymoeObject[] {
																														_state_123,
																														new TinymoeBoolean(false)
																														});
																												}),
																												new TinymoeFunction(__args___x46 => 
																												{
																													TinymoeObject _state_125 = __args___x46[0];
																													TinymoeObject _result_126 = __args___x46[1];
																													standard_library__else(
																														_state_125,
																														_result_126,
																														new TinymoeFunction(__args___x47 => 
																														{
																															TinymoeObject _state_131 = __args___x47[0];
																															TinymoeObject _continuation_132 = __args___x47[1];
																															c = new TinymoeInteger(3);
																															Invoke(_continuation_132, new TinymoeObject[] {
																																_state_131,
																																null
																																});
																														}),
																														new TinymoeFunction(__args___x48 => 
																														{
																															TinymoeObject _state_129 = __args___x48[0];
																															TinymoeObject _result_130 = __args___x48[1];
																															unit_test__assert__expression_should_be__expression(
																																_state_129,
																																a,
																																new TinymoeInteger(1),
																																new TinymoeFunction(__args___x49 => 
																																{
																																	TinymoeObject _state_133 = __args___x49[0];
																																	TinymoeObject _result_134 = __args___x49[1];
																																	unit_test__assert__expression_should_be__expression(
																																		_state_133,
																																		b,
																																		new TinymoeInteger(2),
																																		new TinymoeFunction(__args___x50 => 
																																		{
																																			TinymoeObject _state_135 = __args___x50[0];
																																			TinymoeObject _result_136 = __args___x50[1];
																																			unit_test__assert__expression_should_be__expression(
																																				_state_135,
																																				c,
																																				new TinymoeInteger(3),
																																				_continuation_84
																																				);
																																		})
																																		);
																																})
																																);
																														})
																														);
																												})
																												);
																										})
																										);
																								})
																								);
																						})
																						);
																				})
																				);
																		})
																		);
																})
																);
														})
														);
												}),
												new TinymoeString("Only one branch of the if-else statement will be executed"),
												new TinymoeFunction(__args___x51 => 
												{
													TinymoeObject _state_81 = __args___x51[0];
													TinymoeObject _result_82 = __args___x51[1];
													unit_test__test_case__expression(
														_state_81,
														new TinymoeFunction(__args___x52 => 
														{
															TinymoeObject _state_141 = __args___x52[0];
															TinymoeObject _continuation_142 = __args___x52[1];
															TinymoeObject e = null;
															e = new TinymoeInteger(0);
															TinymoeObject _state_143 = null;
															_state_143 = _state_141;
															standard_library__try(
																_state_143,
																new TinymoeFunction(__args___x53 => 
																{
																	TinymoeObject _state_147 = __args___x53[0];
																	TinymoeObject _continuation_148 = __args___x53[1];
																	standard_library__raise__expression(
																		_state_147,
																		new TinymoeString("exception"),
																		new TinymoeFunction(__args___x54 => 
																		{
																			TinymoeObject _state_149 = __args___x54[0];
																			TinymoeObject _result_150 = __args___x54[1];
																			e = new TinymoeInteger(0);
																			Invoke(_continuation_148, new TinymoeObject[] {
																				_state_149,
																				_result_150
																				});
																		})
																		);
																}),
																new TinymoeFunction(__args___x55 => 
																{
																	TinymoeObject _state_145 = __args___x55[0];
																	TinymoeObject _result_146 = __args___x55[1];
																	standard_library__catch__argument(
																		_state_145,
																		_result_146,
																		new TinymoeFunction(__args___x56 => 
																		{
																			TinymoeObject _state_153 = __args___x56[0];
																			TinymoeObject exception = __args___x56[1];
																			TinymoeObject _continuation_154 = __args___x56[2];
																			e = exception;
																			Invoke(_continuation_154, new TinymoeObject[] {
																				_state_153,
																				null
																				});
																		}),
																		new TinymoeFunction(__args___x57 => 
																		{
																			TinymoeObject _state_151 = __args___x57[0];
																			TinymoeObject _result_152 = __args___x57[1];
																			unit_test__assert__expression_should_be__expression(
																				_state_151,
																				e,
																				new TinymoeString("exception"),
																				_continuation_142
																				);
																		})
																		);
																})
																);
														}),
														new TinymoeString("A raised exception should be able to catch"),
														new TinymoeFunction(__args___x58 => 
														{
															TinymoeObject _state_139 = __args___x58[0];
															TinymoeObject _result_140 = __args___x58[1];
															unit_test__test_case__expression(
																_state_139,
																new TinymoeFunction(__args___x59 => 
																{
																	TinymoeObject _state_159 = __args___x59[0];
																	TinymoeObject _continuation_160 = __args___x59[1];
																	TinymoeObject e_x2 = null;
																	e_x2 = new TinymoeInteger(0);
																	TinymoeObject _state_161 = null;
																	_state_161 = _state_159;
																	standard_library__try(
																		_state_161,
																		new TinymoeFunction(__args___x60 => 
																		{
																			TinymoeObject _state_165 = __args___x60[0];
																			TinymoeObject _continuation_166 = __args___x60[1];
																			standard_library__add__expression_to_assignable(
																				_state_165,
																				new TinymoeInteger(1),
																				new TinymoeFunction(__args___x61 => 
																				{
																					TinymoeObject _state_167 = __args___x61[0];
																					TinymoeObject _continuation_168 = __args___x61[1];
																					Invoke(_continuation_168, new TinymoeObject[] {
																						_state_167,
																						e_x2
																						});
																				}),
																				new TinymoeFunction(__args___x62 => 
																				{
																					TinymoeObject _state_169 = __args___x62[0];
																					TinymoeObject _input_170 = __args___x62[1];
																					TinymoeObject _continuation_171 = __args___x62[2];
																					e_x2 = _input_170;
																					Invoke(_continuation_171, new TinymoeObject[] {
																						_state_169,
																						null
																						});
																				}),
																				new TinymoeFunction(__args___x63 => 
																				{
																					TinymoeObject _state_172 = __args___x63[0];
																					TinymoeObject _result_173 = __args___x63[1];
																					standard_library__raise__expression(
																						_state_172,
																						new TinymoeString("exception"),
																						new TinymoeFunction(__args___x64 => 
																						{
																							TinymoeObject _state_174 = __args___x64[0];
																							TinymoeObject _result_175 = __args___x64[1];
																							standard_library__add__expression_to_assignable(
																								_state_174,
																								new TinymoeInteger(2),
																								new TinymoeFunction(__args___x65 => 
																								{
																									TinymoeObject _state_176 = __args___x65[0];
																									TinymoeObject _continuation_177 = __args___x65[1];
																									Invoke(_continuation_177, new TinymoeObject[] {
																										_state_176,
																										e_x2
																										});
																								}),
																								new TinymoeFunction(__args___x66 => 
																								{
																									TinymoeObject _state_178 = __args___x66[0];
																									TinymoeObject _input_179 = __args___x66[1];
																									TinymoeObject _continuation_180 = __args___x66[2];
																									e_x2 = _input_179;
																									Invoke(_continuation_180, new TinymoeObject[] {
																										_state_178,
																										null
																										});
																								}),
																								_continuation_166
																								);
																						})
																						);
																				})
																				);
																		}),
																		new TinymoeFunction(__args___x67 => 
																		{
																			TinymoeObject _state_163 = __args___x67[0];
																			TinymoeObject _result_164 = __args___x67[1];
																			standard_library__catch__argument(
																				_state_163,
																				_result_164,
																				new TinymoeFunction(__args___x68 => 
																				{
																					TinymoeObject _state_185 = __args___x68[0];
																					TinymoeObject exception_x2 = __args___x68[1];
																					TinymoeObject _continuation_186 = __args___x68[2];
																					standard_library__add__expression_to_assignable(
																						_state_185,
																						new TinymoeInteger(4),
																						new TinymoeFunction(__args___x69 => 
																						{
																							TinymoeObject _state_187 = __args___x69[0];
																							TinymoeObject _continuation_188 = __args___x69[1];
																							Invoke(_continuation_188, new TinymoeObject[] {
																								_state_187,
																								e_x2
																								});
																						}),
																						new TinymoeFunction(__args___x70 => 
																						{
																							TinymoeObject _state_189 = __args___x70[0];
																							TinymoeObject _input_190 = __args___x70[1];
																							TinymoeObject _continuation_191 = __args___x70[2];
																							e_x2 = _input_190;
																							Invoke(_continuation_191, new TinymoeObject[] {
																								_state_189,
																								null
																								});
																						}),
																						_continuation_186
																						);
																				}),
																				new TinymoeFunction(__args___x71 => 
																				{
																					TinymoeObject _state_183 = __args___x71[0];
																					TinymoeObject _result_184 = __args___x71[1];
																					standard_library__finally(
																						_state_183,
																						_result_184,
																						new TinymoeFunction(__args___x72 => 
																						{
																							TinymoeObject _state_196 = __args___x72[0];
																							TinymoeObject _continuation_197 = __args___x72[1];
																							standard_library__add__expression_to_assignable(
																								_state_196,
																								new TinymoeInteger(8),
																								new TinymoeFunction(__args___x73 => 
																								{
																									TinymoeObject _state_198 = __args___x73[0];
																									TinymoeObject _continuation_199 = __args___x73[1];
																									Invoke(_continuation_199, new TinymoeObject[] {
																										_state_198,
																										e_x2
																										});
																								}),
																								new TinymoeFunction(__args___x74 => 
																								{
																									TinymoeObject _state_200 = __args___x74[0];
																									TinymoeObject _input_201 = __args___x74[1];
																									TinymoeObject _continuation_202 = __args___x74[2];
																									e_x2 = _input_201;
																									Invoke(_continuation_202, new TinymoeObject[] {
																										_state_200,
																										null
																										});
																								}),
																								_continuation_197
																								);
																						}),
																						new TinymoeFunction(__args___x75 => 
																						{
																							TinymoeObject _state_194 = __args___x75[0];
																							TinymoeObject _result_195 = __args___x75[1];
																							unit_test__assert__expression_should_be__expression(
																								_state_194,
																								e_x2,
																								new TinymoeInteger(13),
																								_continuation_160
																								);
																						})
																						);
																				})
																				);
																		})
																		);
																}),
																new TinymoeString("Finally should always be executed"),
																new TinymoeFunction(__args___x76 => 
																{
																	TinymoeObject _state_157 = __args___x76[0];
																	TinymoeObject _result_158 = __args___x76[1];
																	unit_test__test_case__expression(
																		_state_157,
																		new TinymoeFunction(__args___x77 => 
																		{
																			TinymoeObject _state_209 = __args___x77[0];
																			TinymoeObject _continuation_210 = __args___x77[1];
																			TinymoeObject a_x2 = null;
																			a_x2 = new TinymoeInteger(0);
																			TinymoeObject _state_211 = null;
																			_state_211 = _state_209;
																			TinymoeObject b_x2 = null;
																			b_x2 = new TinymoeInteger(0);
																			TinymoeObject _state_213 = null;
																			_state_213 = _state_211;
																			TinymoeObject c_x2 = null;
																			c_x2 = new TinymoeInteger(0);
																			TinymoeObject _state_215 = null;
																			_state_215 = _state_213;
																			standard_library__try(
																				_state_215,
																				new TinymoeFunction(__args___x78 => 
																				{
																					TinymoeObject _state_219 = __args___x78[0];
																					TinymoeObject _continuation_220 = __args___x78[1];
																					a_x2 = new TinymoeInteger(1);
																					Invoke(_continuation_220, new TinymoeObject[] {
																						_state_219,
																						null
																						});
																				}),
																				new TinymoeFunction(__args___x79 => 
																				{
																					TinymoeObject _state_217 = __args___x79[0];
																					TinymoeObject _result_218 = __args___x79[1];
																					standard_library__else_try(
																						_state_217,
																						_result_218,
																						new TinymoeFunction(__args___x80 => 
																						{
																							TinymoeObject _state_223 = __args___x80[0];
																							TinymoeObject _continuation_224 = __args___x80[1];
																							a_x2 = new TinymoeInteger(2);
																							Invoke(_continuation_224, new TinymoeObject[] {
																								_state_223,
																								null
																								});
																						}),
																						new TinymoeFunction(__args___x81 => 
																						{
																							TinymoeObject _state_221 = __args___x81[0];
																							TinymoeObject _result_222 = __args___x81[1];
																							standard_library__catch__argument(
																								_state_221,
																								_result_222,
																								new TinymoeFunction(__args___x82 => 
																								{
																									TinymoeObject _state_227 = __args___x82[0];
																									TinymoeObject exception_x3 = __args___x82[1];
																									TinymoeObject _continuation_228 = __args___x82[2];
																									a_x2 = new TinymoeInteger(3);
																									Invoke(_continuation_228, new TinymoeObject[] {
																										_state_227,
																										null
																										});
																								}),
																								new TinymoeFunction(__args___x83 => 
																								{
																									TinymoeObject _state_225 = __args___x83[0];
																									TinymoeObject _result_226 = __args___x83[1];
																									standard_library__try(
																										_state_225,
																										new TinymoeFunction(__args___x84 => 
																										{
																											TinymoeObject _state_231 = __args___x84[0];
																											TinymoeObject _continuation_232 = __args___x84[1];
																											b_x2 = new TinymoeInteger(1);
																											standard_library__raise__expression(
																												_state_231,
																												new TinymoeString("exception"),
																												_continuation_232
																												);
																										}),
																										new TinymoeFunction(__args___x85 => 
																										{
																											TinymoeObject _state_229 = __args___x85[0];
																											TinymoeObject _result_230 = __args___x85[1];
																											standard_library__else_try(
																												_state_229,
																												_result_230,
																												new TinymoeFunction(__args___x86 => 
																												{
																													TinymoeObject _state_237 = __args___x86[0];
																													TinymoeObject _continuation_238 = __args___x86[1];
																													b_x2 = new TinymoeInteger(2);
																													Invoke(_continuation_238, new TinymoeObject[] {
																														_state_237,
																														null
																														});
																												}),
																												new TinymoeFunction(__args___x87 => 
																												{
																													TinymoeObject _state_235 = __args___x87[0];
																													TinymoeObject _result_236 = __args___x87[1];
																													standard_library__catch__argument(
																														_state_235,
																														_result_236,
																														new TinymoeFunction(__args___x88 => 
																														{
																															TinymoeObject _state_241 = __args___x88[0];
																															TinymoeObject exception_x4 = __args___x88[1];
																															TinymoeObject _continuation_242 = __args___x88[2];
																															b_x2 = new TinymoeInteger(3);
																															Invoke(_continuation_242, new TinymoeObject[] {
																																_state_241,
																																null
																																});
																														}),
																														new TinymoeFunction(__args___x89 => 
																														{
																															TinymoeObject _state_239 = __args___x89[0];
																															TinymoeObject _result_240 = __args___x89[1];
																															standard_library__try(
																																_state_239,
																																new TinymoeFunction(__args___x90 => 
																																{
																																	TinymoeObject _state_245 = __args___x90[0];
																																	TinymoeObject _continuation_246 = __args___x90[1];
																																	c_x2 = new TinymoeInteger(1);
																																	standard_library__raise__expression(
																																		_state_245,
																																		new TinymoeString("exception"),
																																		_continuation_246
																																		);
																																}),
																																new TinymoeFunction(__args___x91 => 
																																{
																																	TinymoeObject _state_243 = __args___x91[0];
																																	TinymoeObject _result_244 = __args___x91[1];
																																	standard_library__else_try(
																																		_state_243,
																																		_result_244,
																																		new TinymoeFunction(__args___x92 => 
																																		{
																																			TinymoeObject _state_251 = __args___x92[0];
																																			TinymoeObject _continuation_252 = __args___x92[1];
																																			c_x2 = new TinymoeInteger(2);
																																			standard_library__raise__expression(
																																				_state_251,
																																				new TinymoeString("exception"),
																																				_continuation_252
																																				);
																																		}),
																																		new TinymoeFunction(__args___x93 => 
																																		{
																																			TinymoeObject _state_249 = __args___x93[0];
																																			TinymoeObject _result_250 = __args___x93[1];
																																			standard_library__catch__argument(
																																				_state_249,
																																				_result_250,
																																				new TinymoeFunction(__args___x94 => 
																																				{
																																					TinymoeObject _state_257 = __args___x94[0];
																																					TinymoeObject exception_x5 = __args___x94[1];
																																					TinymoeObject _continuation_258 = __args___x94[2];
																																					c_x2 = new TinymoeInteger(3);
																																					Invoke(_continuation_258, new TinymoeObject[] {
																																						_state_257,
																																						null
																																						});
																																				}),
																																				new TinymoeFunction(__args___x95 => 
																																				{
																																					TinymoeObject _state_255 = __args___x95[0];
																																					TinymoeObject _result_256 = __args___x95[1];
																																					unit_test__assert__expression_should_be__expression(
																																						_state_255,
																																						a_x2,
																																						new TinymoeInteger(1),
																																						new TinymoeFunction(__args___x96 => 
																																						{
																																							TinymoeObject _state_259 = __args___x96[0];
																																							TinymoeObject _result_260 = __args___x96[1];
																																							unit_test__assert__expression_should_be__expression(
																																								_state_259,
																																								b_x2,
																																								new TinymoeInteger(2),
																																								new TinymoeFunction(__args___x97 => 
																																								{
																																									TinymoeObject _state_261 = __args___x97[0];
																																									TinymoeObject _result_262 = __args___x97[1];
																																									unit_test__assert__expression_should_be__expression(
																																										_state_261,
																																										c_x2,
																																										new TinymoeInteger(3),
																																										_continuation_210
																																										);
																																								})
																																								);
																																						})
																																						);
																																				})
																																				);
																																		})
																																		);
																																})
																																);
																														})
																														);
																												})
																												);
																										})
																										);
																								})
																								);
																						})
																						);
																				})
																				);
																		}),
																		new TinymoeString("Only one branch of the try-else statement will be executed"),
																		new TinymoeFunction(__args___x98 => 
																		{
																			TinymoeObject _state_207 = __args___x98[0];
																			TinymoeObject _result_208 = __args___x98[1];
																			unit_test__test_case__expression(
																				_state_207,
																				new TinymoeFunction(__args___x99 => 
																				{
																					TinymoeObject _state_267 = __args___x99[0];
																					TinymoeObject _continuation_268 = __args___x99[1];
																					TinymoeObject e_x3 = null;
																					e_x3 = new TinymoeInteger(0);
																					TinymoeObject _state_269 = null;
																					_state_269 = _state_267;
																					standard_library__named_block__argument(
																						_state_269,
																						new TinymoeFunction(__args___x100 => 
																						{
																							TinymoeObject _state_273 = __args___x100[0];
																							TinymoeObject one = __args___x100[1];
																							TinymoeObject _continuation_274 = __args___x100[2];
																							standard_library__add__expression_to_assignable(
																								_state_273,
																								new TinymoeInteger(1),
																								new TinymoeFunction(__args___x101 => 
																								{
																									TinymoeObject _state_275 = __args___x101[0];
																									TinymoeObject _continuation_276 = __args___x101[1];
																									Invoke(_continuation_276, new TinymoeObject[] {
																										_state_275,
																										e_x3
																										});
																								}),
																								new TinymoeFunction(__args___x102 => 
																								{
																									TinymoeObject _state_277 = __args___x102[0];
																									TinymoeObject _input_278 = __args___x102[1];
																									TinymoeObject _continuation_279 = __args___x102[2];
																									e_x3 = _input_278;
																									Invoke(_continuation_279, new TinymoeObject[] {
																										_state_277,
																										null
																										});
																								}),
																								new TinymoeFunction(__args___x103 => 
																								{
																									TinymoeObject _state_280 = __args___x103[0];
																									TinymoeObject _result_281 = __args___x103[1];
																									standard_library__named_block__argument(
																										_state_280,
																										new TinymoeFunction(__args___x104 => 
																										{
																											TinymoeObject _state_284 = __args___x104[0];
																											TinymoeObject two = __args___x104[1];
																											TinymoeObject _continuation_285 = __args___x104[2];
																											standard_library__add__expression_to_assignable(
																												_state_284,
																												new TinymoeInteger(2),
																												new TinymoeFunction(__args___x105 => 
																												{
																													TinymoeObject _state_286 = __args___x105[0];
																													TinymoeObject _continuation_287 = __args___x105[1];
																													Invoke(_continuation_287, new TinymoeObject[] {
																														_state_286,
																														e_x3
																														});
																												}),
																												new TinymoeFunction(__args___x106 => 
																												{
																													TinymoeObject _state_288 = __args___x106[0];
																													TinymoeObject _input_289 = __args___x106[1];
																													TinymoeObject _continuation_290 = __args___x106[2];
																													e_x3 = _input_289;
																													Invoke(_continuation_290, new TinymoeObject[] {
																														_state_288,
																														null
																														});
																												}),
																												new TinymoeFunction(__args___x107 => 
																												{
																													TinymoeObject _state_291 = __args___x107[0];
																													TinymoeObject _result_292 = __args___x107[1];
																													standard_library__named_block__argument(
																														_state_291,
																														new TinymoeFunction(__args___x108 => 
																														{
																															TinymoeObject _state_295 = __args___x108[0];
																															TinymoeObject three = __args___x108[1];
																															TinymoeObject _continuation_296 = __args___x108[2];
																															standard_library__add__expression_to_assignable(
																																_state_295,
																																new TinymoeInteger(4),
																																new TinymoeFunction(__args___x109 => 
																																{
																																	TinymoeObject _state_297 = __args___x109[0];
																																	TinymoeObject _continuation_298 = __args___x109[1];
																																	Invoke(_continuation_298, new TinymoeObject[] {
																																		_state_297,
																																		e_x3
																																		});
																																}),
																																new TinymoeFunction(__args___x110 => 
																																{
																																	TinymoeObject _state_299 = __args___x110[0];
																																	TinymoeObject _input_300 = __args___x110[1];
																																	TinymoeObject _continuation_301 = __args___x110[2];
																																	e_x3 = _input_300;
																																	Invoke(_continuation_301, new TinymoeObject[] {
																																		_state_299,
																																		null
																																		});
																																}),
																																new TinymoeFunction(__args___x111 => 
																																{
																																	TinymoeObject _state_302 = __args___x111[0];
																																	TinymoeObject _result_303 = __args___x111[1];
																																	standard_library__exit_block__expression(
																																		_state_302,
																																		two,
																																		new TinymoeFunction(__args___x112 => 
																																		{
																																			TinymoeObject _state_304 = __args___x112[0];
																																			TinymoeObject _result_305 = __args___x112[1];
																																			standard_library__add__expression_to_assignable(
																																				_state_304,
																																				new TinymoeInteger(8),
																																				new TinymoeFunction(__args___x113 => 
																																				{
																																					TinymoeObject _state_306 = __args___x113[0];
																																					TinymoeObject _continuation_307 = __args___x113[1];
																																					Invoke(_continuation_307, new TinymoeObject[] {
																																						_state_306,
																																						e_x3
																																						});
																																				}),
																																				new TinymoeFunction(__args___x114 => 
																																				{
																																					TinymoeObject _state_308 = __args___x114[0];
																																					TinymoeObject _input_309 = __args___x114[1];
																																					TinymoeObject _continuation_310 = __args___x114[2];
																																					e_x3 = _input_309;
																																					Invoke(_continuation_310, new TinymoeObject[] {
																																						_state_308,
																																						null
																																						});
																																				}),
																																				_continuation_296
																																				);
																																		})
																																		);
																																})
																																);
																														}),
																														new TinymoeFunction(__args___x115 => 
																														{
																															TinymoeObject _state_293 = __args___x115[0];
																															TinymoeObject _result_294 = __args___x115[1];
																															standard_library__add__expression_to_assignable(
																																_state_293,
																																new TinymoeInteger(16),
																																new TinymoeFunction(__args___x116 => 
																																{
																																	TinymoeObject _state_313 = __args___x116[0];
																																	TinymoeObject _continuation_314 = __args___x116[1];
																																	Invoke(_continuation_314, new TinymoeObject[] {
																																		_state_313,
																																		e_x3
																																		});
																																}),
																																new TinymoeFunction(__args___x117 => 
																																{
																																	TinymoeObject _state_315 = __args___x117[0];
																																	TinymoeObject _input_316 = __args___x117[1];
																																	TinymoeObject _continuation_317 = __args___x117[2];
																																	e_x3 = _input_316;
																																	Invoke(_continuation_317, new TinymoeObject[] {
																																		_state_315,
																																		null
																																		});
																																}),
																																_continuation_285
																																);
																														})
																														);
																												})
																												);
																										}),
																										new TinymoeFunction(__args___x118 => 
																										{
																											TinymoeObject _state_282 = __args___x118[0];
																											TinymoeObject _result_283 = __args___x118[1];
																											standard_library__add__expression_to_assignable(
																												_state_282,
																												new TinymoeInteger(32),
																												new TinymoeFunction(__args___x119 => 
																												{
																													TinymoeObject _state_320 = __args___x119[0];
																													TinymoeObject _continuation_321 = __args___x119[1];
																													Invoke(_continuation_321, new TinymoeObject[] {
																														_state_320,
																														e_x3
																														});
																												}),
																												new TinymoeFunction(__args___x120 => 
																												{
																													TinymoeObject _state_322 = __args___x120[0];
																													TinymoeObject _input_323 = __args___x120[1];
																													TinymoeObject _continuation_324 = __args___x120[2];
																													e_x3 = _input_323;
																													Invoke(_continuation_324, new TinymoeObject[] {
																														_state_322,
																														null
																														});
																												}),
																												_continuation_274
																												);
																										})
																										);
																								})
																								);
																						}),
																						new TinymoeFunction(__args___x121 => 
																						{
																							TinymoeObject _state_271 = __args___x121[0];
																							TinymoeObject _result_272 = __args___x121[1];
																							standard_library__add__expression_to_assignable(
																								_state_271,
																								new TinymoeInteger(64),
																								new TinymoeFunction(__args___x122 => 
																								{
																									TinymoeObject _state_327 = __args___x122[0];
																									TinymoeObject _continuation_328 = __args___x122[1];
																									Invoke(_continuation_328, new TinymoeObject[] {
																										_state_327,
																										e_x3
																										});
																								}),
																								new TinymoeFunction(__args___x123 => 
																								{
																									TinymoeObject _state_329 = __args___x123[0];
																									TinymoeObject _input_330 = __args___x123[1];
																									TinymoeObject _continuation_331 = __args___x123[2];
																									e_x3 = _input_330;
																									Invoke(_continuation_331, new TinymoeObject[] {
																										_state_329,
																										null
																										});
																								}),
																								new TinymoeFunction(__args___x124 => 
																								{
																									TinymoeObject _state_332 = __args___x124[0];
																									TinymoeObject _result_333 = __args___x124[1];
																									unit_test__assert__expression_should_be__expression(
																										_state_332,
																										e_x3,
																										new TinymoeInteger(103),
																										_continuation_268
																										);
																								})
																								);
																						})
																						);
																				}),
																				new TinymoeString("Exit block should jump out of the correct box"),
																				new TinymoeFunction(__args___x125 => 
																				{
																					TinymoeObject _state_265 = __args___x125[0];
																					TinymoeObject _result_266 = __args___x125[1];
																					Invoke(_continuation, new TinymoeObject[] {
																						_state,
																						_the_result
																						});
																				})
																				);
																		})
																		);
																})
																);
														})
														);
												})
												);
										})
										);
								})
								);
						})
						);
				})
				);
		}

		public void _dispatch_fail__standard_library__repeat_with__argument_in__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				new TinymoeString("Only enumerable object (like containers) can be iterated using the repeat-with-in statement."),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void TinymoeArray___dispatch__standard_library__repeat_with__argument_in__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			standard_library__repeat_with__argument_in__expression_array_(
				_state,
				deal_with__expression,
				items,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__repeat_with__argument_in__expression_x2(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__repeat_with__argument_in__expression(
				_state,
				deal_with__expression,
				items,
				_continuation
				);
		}

		public TinymoeProgram()
		{
			TinymoeObject.SetExtension(
				typeof(TinymoeArray),
				"$dispatch<>standard_library::repeat_with_$argument_in_$expression",
				new TinymoeFunction(__args__ => TinymoeArray___dispatch__standard_library__repeat_with__argument_in__expression(__args__[0], __args__[1], __args__[2], __args__[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::repeat_with_$argument_in_$expression",
				new TinymoeFunction(__args___x2 => TinymoeObject___dispatch__standard_library__repeat_with__argument_in__expression_x2(__args___x2[0], __args___x2[1], __args___x2[2], __args___x2[3]))
				); 
		}

		static void Main(string[] args)
		{
			var program = new TinymoeProgram();
			var continuation = new TinymoeFunction((TinymoeObject[] arguments) =>
			{
			});
			var trap = new TinymoeProgram.standard_library__continuation_trap();
			trap.SetField("continuation", continuation);
			var state = new TinymoeProgram.standard_library__continuation_state();
			state.SetField("trap", trap);
			program.unit_test__main(state, continuation);
		}
	}
}
