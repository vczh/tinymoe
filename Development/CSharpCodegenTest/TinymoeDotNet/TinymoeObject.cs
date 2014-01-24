using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace TinymoeDotNet
{
    public class TinymoeObject
    {
        private static UInt64 counter = 0;
        internal Dictionary<string, TinymoeObject> fields = new Dictionary<string, TinymoeObject>();
        private List<string> fieldNames = new List<string>();
        private bool finishedConstruction = false;
        private UInt64 id = counter++;

        public void SetField(string name, TinymoeObject value)
        {
            if (finishedConstruction)
            {
                if (!this.fieldNames.Contains(name))
                {
                    throw new ArgumentOutOfRangeException("name");
                }
                this.fields[name] = value;
            }
            else
            {
                this.fields[name] = value;
                this.fieldNames.Add(name);
            }
        }

        public TinymoeObject FinishConstruction()
        {
            this.finishedConstruction = true;
            return this;
        }

        public TinymoeObject SetFields(TinymoeObject[] values)
        {
            foreach (var x in this.fieldNames.Zip(values, Tuple.Create))
            {
                this.fields[x.Item1] = x.Item2;
            }
            return this;
        }

        public UInt64 Id
        {
            get
            {
                return this.id;
            }
        }

        public override string ToString()
        {
            return "(" + this.GetType().Name + ")" + id.ToString();
        }
    }

    public class TinymoeBoolean : TinymoeObject
    {
        public bool Value { get; private set; }

        public TinymoeBoolean(bool value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(boolean)" + Value.ToString();
        }
    }

    public class TinymoeInteger : TinymoeObject
    {
        public int Value { get; private set; }

        public TinymoeInteger(int value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(integer)" + Value.ToString();
        }
    }

    public class TinymoeFloat : TinymoeObject
    {
        public double Value { get; private set; }

        public TinymoeFloat(double value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(float)" + Value.ToString();
        }
    }

    public class TinymoeString : TinymoeObject
    {
        public string Value { get; private set; }

        public TinymoeString(string value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(string)" + Value.ToString();
        }
    }

    public class TinymoeSymbol : TinymoeObject
    {
        public string Value { get; private set; }

        public TinymoeSymbol(string value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(symbol)" + Value.ToString();
        }
    }

    public class TinymoeArray : TinymoeObject
    {
        public TinymoeObject[] Elements { get; private set; }

        public TinymoeArray(int length)
        {
            this.Elements = new TinymoeObject[length];
        }

        public TinymoeArray(TinymoeObject[] values)
        {
            this.Elements = values.ToArray();
        }

        public override string ToString()
        {
            return "[" + Elements.Aggregate("", (a, b) => a + (a == "" ? "" : ",") + b.ToString()) + "]";
        }
    }

    public class TinymoeFunction : TinymoeObject
    {
        public Action<TinymoeObject[]> Handler { get; private set; }

        public TinymoeFunction(Action<TinymoeObject[]> value)
        {
            this.Handler = value;
        }

        public override string ToString()
        {
            return "(function)";
        }
    }

    public class TinymoeOperations
    {
        static readonly Dictionary<Tuple<Type, string>, TinymoeObject> extensions = new Dictionary<Tuple<Type, string>, TinymoeObject>();
        static TinymoeOperations()
        {
        }

        public static void Invoke(TinymoeObject function, TinymoeObject[] arguments)
        {
            ((TinymoeFunction)function).Handler(arguments);
        }

        public static void SetExtension(Type type, string name, TinymoeObject value)
        {
            extensions[Tuple.Create(type, name)] = value;
        }

        public TinymoeObject GetField(TinymoeObject target, string name)
        {
            TinymoeObject value = null;
            if (target.fields.TryGetValue(name, out value))
            {
                return value;
            }

            Type type = target.GetType();
            while (type != null)
            {
                if (extensions.TryGetValue(Tuple.Create(type, name), out value))
                {
                    return value;
                }
                type = type.BaseType;
            }

            throw new ArgumentOutOfRangeException("name");
        }

        public static TinymoeObject ArrayLength(TinymoeObject array)
        {
            return new TinymoeInteger(((TinymoeArray)array).Elements.Length);
        }

        public static TinymoeObject ArrayGet(TinymoeObject array, TinymoeObject index)
        {
            return ((TinymoeArray)array).Elements[CastToInteger(index).Value];
        }

        public static void ArraySet(TinymoeObject array, TinymoeObject index, TinymoeObject value)
        {
            ((TinymoeArray)array).Elements[CastToInteger(index).Value] = value;
        }

        private static readonly Dictionary<string, TinymoeObject> externalFunctions = new Dictionary<string, TinymoeObject>();

        public static TinymoeObject BuildExternalFunction(Func<TinymoeObject[], TinymoeObject> function)
        {
            return new TinymoeFunction(__args__ =>
            {
                var result = function(__args__.Skip(1).Take(__args__.Length - 2).ToArray());
                Invoke(__args__[2], new TinymoeObject[] { __args__[0], result });
            });
        }

        #region STRONG TYPED EXTERNAL FUNCTIONS

        private static int f_to_i(double v)
        {
            return (int)v;
        }

        private static int s_to_i(string v)
        {
            return int.Parse(v);
        }

        private static double i_to_f(int v)
        {
            return (double)v;
        }

        private static double s_to_f(string v)
        {
            return double.Parse(v);
        }

        private static int pos_i(int v)
        {
            return v;
        }

        private static double pos_f(double v)
        {
            return v;
        }

        private static int neg_i(int v)
        {
            return -v;
        }

        private static double neg_f(double v)
        {
            return -v;
        }

        private static bool not_b(bool v)
        {
            return !v;
        }

        private static string s_concat_s(string a, string b)
        {
            return a + b;
        }

        private static int i_add_i(int a, int b)
        {
            return a + b;
        }

        private static double f_add_f(double a, double b)
        {
            return a + b;
        }

        private static int i_sub_i(int a, int b)
        {
            return a - b;
        }

        private static double f_sub_f(double a, double b)
        {
            return a - b;
        }

        private static int i_mul_i(int a, int b)
        {
            return a * b;
        }

        private static double f_mul_f(double a, double b)
        {
            return a * b;
        }

        private static double i_div_i(int a, int b)
        {
            return (double)a / (double)b;
        }

        private static double f_div_f(double a, double b)
        {
            return a / b;
        }

        private static int i_intdiv_i(int a, int b)
        {
            return a / b;
        }

        private static int i_mod_i(int a, int b)
        {
            return a % b;
        }

        private static bool o_e_o(TinymoeObject a, TinymoeObject b)
        {
            return a == b;
        }

        private static bool i_e_i(int a, int b)
        {
            return a == b;
        }

        private static bool f_e_f(double a, double b)
        {
            return a == b;
        }

        private static bool s_e_s(string a, string b)
        {
            return a == b;
        }

        private static bool b_e_b(bool a, bool b)
        {
            return a == b;
        }

        private static int i_c_i(int a, int b)
        {
            return a < b ? -1 : a > b ? 1 : 0;
        }

        private static int f_c_f(double a, double b)
        {
            return a < b ? -1 : a > b ? 1 : 0;
        }

        private static int s_c_s(string a, string b)
        {
            return i_c_i(a.CompareTo(b), 0);
        }

        private static bool b_and_b(bool a, bool b)
        {
            return a && b;
        }

        private static bool b_or_b(bool a, bool b)
        {
            return a || b;
        }

        private static object FromTinymoe(TinymoeObject v, Type type)
        {
            if (type == typeof(bool))
            {
                return ((TinymoeBoolean)v).Value;
            }
            else if (type == typeof(int))
            {
                return ((TinymoeInteger)v).Value;
            }
            else if (type == typeof(double))
            {
                return ((TinymoeFloat)v).Value;
            }
            else if (type == typeof(string))
            {
                return ((TinymoeString)v).Value;
            }
            else if (type == typeof(TinymoeObject))
            {
                return v;
            }
            else
            {
                throw new ArgumentException(string.Format("A TinymoeObject cannot be converted to A value of {0}.", v.GetType().FullName));
            }
        }

        private static TinymoeObject ToTinymoe(object v)
        {
            if (v is bool)
            {
                return new TinymoeBoolean((bool)v);
            }
            else if (v is int)
            {
                return new TinymoeInteger((int)v);
            }
            else if (v is double)
            {
                return new TinymoeFloat((double)v);
            }
            else if (v is string)
            {
                return new TinymoeString((string)v);
            }
            else if (v is TinymoeObject)
            {
                return (TinymoeObject)v;
            }
            else
            {
                throw new ArgumentException(string.Format("A value of {0} cannot be converted to a TinymoeObject", v.GetType().FullName));
            }
        }

        private static TinymoeObject BuildStringTypedExternalFunction(MethodInfo function)
        {
            return new TinymoeFunction(__args__ =>
            {
                var arguments = __args__
                    .Skip(1)
                    .Take(__args__.Length - 2)
                    .Select((v, i) => FromTinymoe(v, function.GetParameters()[i].ParameterType))
                    .ToArray();
                var result = function.Invoke(null, arguments);
                Invoke(__args__.Last(), new TinymoeObject[] { __args__[0], ToTinymoe(result) });
            });
        }

        #endregion

        public static TinymoeObject GetExternalFunction(string name)
        {
            TinymoeObject function;
            if (!externalFunctions.TryGetValue(name, out function))
            {
                switch (name)
                {
                    case "s_to_n":
                        function = BuildExternalFunction(__args__ => CastToNumber(__args__[0]));
                        break;
                    case "to_s":
                        function = BuildExternalFunction(__args__ => CastToString(__args__[0]));
                        break;
                    case "Print":
                        function = BuildExternalFunction(__args__ => Print(__args__[0]));
                        break;
                    case "Sqrt":
                        function = BuildExternalFunction(__args__ => Sqrt(__args__[0]));
                        break;
                    default:
                        {
                            var method = typeof(TinymoeOperations).GetMethod(name, BindingFlags.Static | BindingFlags.NonPublic);
                            if (method == null)
                            {
                                throw new IndexOutOfRangeException(string.Format("External function \"{0}\" does not exist.", name));
                            }
                            function = BuildStringTypedExternalFunction(method);
                        }
                        break;
                }
                externalFunctions.Add(name, function);
            }
            return function;
        }

        public static TinymoeBoolean CastToBoolean(TinymoeObject a)
        {
            return (TinymoeBoolean)a;
        }

        private static TinymoeObject CastToNumber(TinymoeObject a)
        {
            if (a is TinymoeInteger || a is TinymoeFloat)
            {
                return a;
            }
            else
            {
                var value = (TinymoeString)a;
                int i;
                if (int.TryParse(value.Value, out i))
                {
                    return new TinymoeInteger(i);
                }
                else
                {
                    return new TinymoeFloat(double.Parse(value.Value));
                }
            }
        }

        private static TinymoeInteger CastToInteger(TinymoeObject a)
        {
            if (a is TinymoeInteger)
            {
                return (TinymoeInteger)a;
            }
            else if (a is TinymoeFloat)
            {
                return new TinymoeInteger((int)((TinymoeFloat)a).Value);
            }
            else
            {
                return new TinymoeInteger(int.Parse(((TinymoeString)a).Value));
            }
        }

        private static TinymoeFloat CastToFloat(TinymoeObject a)
        {
            if (a is TinymoeInteger)
            {
                return new TinymoeFloat((double)((TinymoeInteger)a).Value);
            }
            else if (a is TinymoeFloat)
            {
                return (TinymoeFloat)a;
            }
            else
            {
                return new TinymoeFloat(double.Parse(((TinymoeString)a).Value));
            }
        }

        private static TinymoeString CastToString(TinymoeObject a)
        {
            if (a is TinymoeInteger)
            {
                return new TinymoeString(((TinymoeInteger)a).Value.ToString());
            }
            else if (a is TinymoeFloat)
            {
                return new TinymoeString(((TinymoeFloat)a).Value.ToString());
            }
            else if (a is TinymoeBoolean)
            {
                return new TinymoeString(((TinymoeBoolean)a).Value.ToString());
            }
            else if (a is TinymoeSymbol)
            {
                return new TinymoeString(((TinymoeSymbol)a).Value.ToString());
            }
            else if (a is TinymoeString)
            {
                return (TinymoeString)a;
            }
            else if (a is TinymoeArray)
            {
                return new TinymoeString("<array>");
            }
            else if (a is TinymoeFunction)
            {
                return new TinymoeString("<function>");
            }
            else
            {
                return new TinymoeString(string.Format("<{0}>", a.GetType().Name));
            }
        }

        private static TinymoeObject Print(TinymoeObject a)
        {
            var value = CastToString(a);
            Console.WriteLine(value.Value);
            return null;
        }

        private static TinymoeObject Sqrt(TinymoeObject a)
        {
            var value = CastToFloat(a);
            return new TinymoeFloat(Math.Sqrt(value.Value));
        }
    }
}
